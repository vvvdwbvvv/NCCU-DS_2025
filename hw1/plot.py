from __future__ import annotations

import argparse
import csv
from pathlib import Path
from dataclasses import dataclass
from typing import Dict, Iterable, List, Literal, Sequence

import matplotlib.pyplot as plt
from matplotlib.lines import Line2D

# Clean publication style
plt.style.use("seaborn-v0_8-whitegrid")
plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.sans-serif': ['Arial', 'Helvetica', 'DejaVu Sans'],
    'font.size': 14,
    'axes.titlesize': 16,
    'axes.labelsize': 14,
    'xtick.labelsize': 12,
    'ytick.labelsize': 12,
    'legend.fontsize': 12,
    'figure.titlesize': 18,
})

STRUCTURE_COLORS: Dict[str, str] = {
    # Publication-ready ColorBrewer-inspired palette
    "DS1":                   "#418af7",  # Blue
    "DS2":                   "#5fb075",  # Green
    "DS3":                   "#ee613d",  # Red
}

STRUCTURE_MARKERS: Dict[str, str] = {
    "DS1": "o",
    "DS2": "v",
    "DS3": "s",
    "DynamicArrayStore": "D",
    "StaticArrayLinkedStore": "^",
}

REQUIRED_COLUMNS: set[str] = {
    "k",
    "n",
    "structure",
    "avg_insert_ms",
    "avg_search_ms",
    "avg_sum_ms",
    "insert_estimated",
    "search_estimated",
    "sum_estimated",
}

METRIC_SPECS = {
    "avg_insert_ms": {
        "title": "Insert time vs N",
        "ylabel": "Time (ms)",
        "flag": "insert_estimated",
        "suffix": "insert",
    },
    "avg_search_ms": {
        "title": "Search (100k) time vs N",
        "ylabel": "Time (ms)",
        "flag": "search_estimated",
        "suffix": "search",
    },
    "avg_sum_ms": {
        "title": "Sum-all-scores time vs N",
        "ylabel": "Time (ms)",
        "flag": "sum_estimated",
        "suffix": "sum",
    },
}

@dataclass
class Record:
    k: int
    n: int
    structure: str
    avg_insert_ms: float
    avg_search_ms: float
    avg_sum_ms: float
    insert_estimated: bool
    search_estimated: bool
    sum_estimated: bool


def detect_schema(fieldnames: Sequence[str]) -> Literal["verbose", "compact"]:
    """Return which supported schema the CSV uses."""
    fields = set(fieldnames)
    if REQUIRED_COLUMNS.issubset(fields):
        return "verbose"
    compact_required = {"Type", "k", "n", "insert", "search100k", "sum", "estimated"}
    if compact_required.issubset(fields):
        return "compact"
    missing_verbose = sorted(REQUIRED_COLUMNS - fields)
    missing_compact = sorted(compact_required - fields)
    raise ValueError(
        "CSV missing required columns. Supported schemas are:\n"
        f"- Verbose: missing {missing_verbose}\n"
        f"- Compact: missing {missing_compact}"
    )


def parse_bool(value) -> bool:
    """Interpret CSV truthy strings (0/1, true/false, yes/no)."""
    if isinstance(value, bool):
        return value
    if value is None:
        return False
    text = str(value).strip().lower()
    if text in {"1", "true", "yes", "y"}:
        return True
    if text in {"0", "false", "no", "n", ""}:
        return False
    raise ValueError(f"Cannot interpret boolean value: {value!r}")


def parse_row_verbose(row: dict, line_no: int) -> Record:
    try:
        k_val = int(row["k"])  # type: ignore[index]
        n_val = int(row["n"])  # type: ignore[index]
    except (TypeError, ValueError) as exc:
        raise ValueError(f"Invalid k or n at line {line_no}: {row}") from exc

    metrics: Dict[str, float] = {}
    flags: Dict[str, bool] = {}
    for metric, spec in METRIC_SPECS.items():
        try:
            metrics[metric] = float(row[metric])  # type: ignore[index]
        except (TypeError, ValueError) as exc:
            raise ValueError(
                f"Invalid numeric value for {metric} at line {line_no}: {row.get(metric)}"
            ) from exc
        try:
            flags[spec["flag"]] = parse_bool(row[spec["flag"]])  # type: ignore[index]
        except ValueError as exc:
            raise ValueError(
                f"Invalid flag value for {spec['flag']} at line {line_no}: {row.get(spec['flag'])}"
            ) from exc

    return Record(
        k=k_val,
        n=n_val,
        structure=str(row["structure"]),  # type: ignore[index]
        avg_insert_ms=metrics["avg_insert_ms"],
        avg_search_ms=metrics["avg_search_ms"],
        avg_sum_ms=metrics["avg_sum_ms"],
        insert_estimated=flags["insert_estimated"],
        search_estimated=flags["search_estimated"],
        sum_estimated=flags["sum_estimated"],
    )


def parse_row_compact(row: dict, line_no: int) -> Record:
    try:
        k_val = int(row["k"])  # type: ignore[index]
        n_val = int(row["n"])  # type: ignore[index]
    except (TypeError, ValueError) as exc:
        raise ValueError(f"Invalid k or n at line {line_no}: {row}") from exc

    try:
        insert_val = float(row["insert"])  # type: ignore[index]
        search_val = float(row["search100k"])  # type: ignore[index]
        sum_val = float(row["sum"])  # type: ignore[index]
    except (TypeError, ValueError) as exc:
        raise ValueError(
            f"Invalid numeric value in compact schema at line {line_no}: "
            f"insert={row.get('insert')}, search100k={row.get('search100k')}, sum={row.get('sum')}"
        ) from exc

    try:
        est_val = parse_bool(row["estimated"])  # type: ignore[index]
    except ValueError as exc:
        raise ValueError(
            f"Invalid flag value for estimated at line {line_no}: {row.get('estimated')}"
        ) from exc

    return Record(
        k=k_val,
        n=n_val,
        structure=str(row["Type"]),  # type: ignore[index]
        avg_insert_ms=insert_val,
        avg_search_ms=search_val,
        avg_sum_ms=sum_val,
        insert_estimated=est_val,
        search_estimated=est_val,
        sum_estimated=est_val,
    )


def load_records(csv_path: Path) -> List[Record]:
    """Read and type-coerce rows from the benchmark CSV."""
    with csv_path.open(newline="") as handle:
        reader = csv.DictReader(handle)
        if reader.fieldnames is None:
            raise ValueError("CSV is missing a header row.")
        schema = detect_schema(reader.fieldnames)

        records: List[Record] = []
        for line_no, row in enumerate(reader, start=2):
            if schema == "verbose":
                records.append(parse_row_verbose(row, line_no))
            else:
                records.append(parse_row_compact(row, line_no))
    return records


def group_by_structure(records: Iterable[Record]) -> Dict[str, List[Record]]:
    grouped: Dict[str, List[Record]] = {}
    for record in records:
        grouped.setdefault(record.structure, []).append(record)
    for rows in grouped.values():
        rows.sort(key=lambda r: r.n)
    return grouped


def build_color_map(structure_names: Sequence[str]) -> Dict[str, str]:
    """Create a deterministic color map for all structures.

    Uses STRUCTURE_COLORS as seeds, and fills remaining names from the
    current Matplotlib color cycle to keep colors consistent across figures.
    """
    color_map: Dict[str, str] = {}
    # Seed with known mappings
    for name in structure_names:
        if name in STRUCTURE_COLORS:
            color_map[name] = STRUCTURE_COLORS[name]
    # Fill the rest from a robust, deterministic palette
    palette = [
        "#377eb8", "#ff7f00", "#4daf4a", "#f781bf", "#a65628",
        "#984ea3", "#999999", "#e41a1c", "#dede00",
    ]
    idx = 0
    for name in structure_names:
        if name in color_map:
            continue
        color_map[name] = palette[idx % len(palette)]
        idx += 1
    return color_map

def build_marker_map(structure_names: Sequence[str]) -> Dict[str, str]:
    """Create a deterministic marker map for all structures."""
    marker_map: Dict[str, str] = {}
    # Seed with known mappings
    for name in structure_names:
        if name in STRUCTURE_MARKERS:
            marker_map[name] = STRUCTURE_MARKERS[name]
    # Fill the rest from a robust palette
    palette = ["o", "v", "s", "D", "^", "P", "X", "*"]
    idx = 0
    for name in structure_names:
        if name in marker_map:
            continue
        marker_map[name] = palette[idx % len(palette)]
        idx += 1
    return marker_map

def render_metric(ax, structure_data: Dict[str, List[Record]], metric_key: str, overlay: bool, color_map: Dict[str, str], marker_map: Dict[str, str]) -> bool:
    spec = METRIC_SPECS[metric_key]
    overlay_used = False
    for structure in sorted(structure_data):
        rows = structure_data[structure]
        xs = [row.n for row in rows]
        ys = [getattr(row, metric_key) for row in rows]
        color = color_map.get(structure)
        marker = marker_map.get(structure)
        # Thicker lines and larger markers for readability
        (line,) = ax.plot(xs, ys, marker=marker, markersize=8, linestyle="-", linewidth=2.0, label=structure, color=color, markeredgewidth=1.5, markeredgecolor=color)
        if overlay:
            est_x = [row.n for row in rows if getattr(row, spec["flag"]) ]
            if est_x:
                est_y = [getattr(row, metric_key) for row in rows if getattr(row, spec["flag"]) ]
                ax.scatter(
                    est_x,
                    est_y,
                    s=120, # Larger scatter marker size
                    marker=marker,
                    edgecolors=line.get_color(),
                    facecolors="none",
                    linewidths=2.5, # Thicker edge
                    zorder=10,
                )
                overlay_used = True
    ax.set_xlabel("N (number of inserted records)")
    ax.set_ylabel(spec["ylabel"])
    ax.set_title(spec["title"])
    # Enhanced grid styling
    ax.grid(True, which='major', linestyle='-', linewidth=0.7, alpha=0.5)
    ax.minorticks_on()
    ax.grid(True, which='minor', linestyle=':', linewidth=0.5, alpha=0.3)
    ax.set_axisbelow(True)
    return overlay_used


def estimated_handle() -> Line2D:
    return Line2D(
        [0],
        [0],
        marker="o",
        linestyle="none",
        markerfacecolor="none",
        markeredgecolor="black",
        markersize=8,
        markeredgewidth=2,
        label="Estimated",
    )


def attach_axis_legend(ax, overlay_used: bool, overlay: bool) -> None:
    handles: List[Line2D] = []
    labels: List[str] = []
    for line in ax.lines:
        label = line.get_label()
        if not label or label == "_nolegend_":
            continue
        handles.append(line)
        labels.append(label)
    if overlay and overlay_used:
        handles.append(estimated_handle())
        labels.append("Estimated")
    if handles:
        ax.legend(handles, labels, loc="best", frameon=False)


def plot_metric_figure(structure_data: Dict[str, List[Record]], metric_key: str, out_path: Path, overlay: bool, color_map: Dict[str, str], marker_map: Dict[str, str]) -> None:
    fig, ax = plt.subplots(figsize=(10, 7))
    overlay_used = render_metric(ax, structure_data, metric_key, overlay, color_map, marker_map)
    attach_axis_legend(ax, overlay_used, overlay)
    fig.tight_layout(pad=1.5)
    fig.savefig(out_path, dpi=160)
    # Also save vector PDF for publication quality
    fig.savefig(out_path.with_suffix(".pdf"))
    plt.close(fig)


def plot_overview(structure_data: Dict[str, List[Record]], out_path: Path, overlay: bool, color_map: Dict[str, str], marker_map: Dict[str, str]) -> None:
    metrics = list(METRIC_SPECS.keys())
    fig, axes = plt.subplots(1, len(metrics), figsize=(20, 6), sharex=True)
    try:
        axes_iterable = list(axes.ravel())  # type: ignore[attr-defined]
    except AttributeError:
        axes_iterable = [axes]

    overlay_flags: List[bool] = []
    for idx, (ax, metric_key) in enumerate(zip(axes_iterable, metrics)):
        overlay_used = render_metric(ax, structure_data, metric_key, overlay, color_map, marker_map)
        overlay_flags.append(overlay_used)
        if idx > 0:
            ax.set_ylabel("")

    # Build a single legend for the entire figure using the first axis.
    handles: List[Line2D] = []
    labels: List[str] = []
    reference_axis = axes_iterable[0]
    for line in reference_axis.lines:
        label = line.get_label()
        if not label or label == "_nolegend_":
            continue
        handles.append(line)
        labels.append(label)
    if overlay and any(overlay_flags):
        handles.append(estimated_handle())
        labels.append("Estimated")
    if handles:
        fig.legend(
            handles,
            labels,
            loc="upper center",
            bbox_to_anchor=(0.5, 1.0),
            ncol=max(1, len(handles)),
            frameon=False,
        )
    fig.tight_layout(rect=(0, 0, 1, 0.90))
    fig.savefig(out_path, dpi=160)
    # Also save vector PDF for publication quality
    fig.savefig(out_path.with_suffix(".pdf"))
    plt.close(fig)


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate charts from benchmark CSV data.")
    parser.add_argument("csv_path", help="Path to benchmark_results.csv")
    parser.add_argument(
        "--overlay",
        action="store_true",
        help="Highlight estimated points with hollow markers.",
    )
    parser.add_argument(
        "--prefix",
        default="fig",
        help="Prefix for generated output files (default: fig).",
    )
    parser.add_argument(
        "--out-dir",
        type=Path,
        default=Path("."),
        help="Directory to write figures into (default: current directory).",
    )
    parser.add_argument(
        "--overview",
        action="store_true",
        help="Produce a combined overview figure with three subplots.",
    )
    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    csv_path = Path(args.csv_path)
    if not csv_path.exists():
        raise SystemExit(f"ERROR: {csv_path} not found")

    records = load_records(csv_path)
    if not records:
        raise SystemExit(f"ERROR: {csv_path} does not contain any data rows")

    structure_data = group_by_structure(records)
    structure_names = sorted(structure_data.keys())
    color_map = build_color_map(structure_names)
    marker_map = build_marker_map(structure_names)

    out_dir: Path = args.out_dir if isinstance(args.out_dir, Path) else Path(args.out_dir)
    out_dir.mkdir(exist_ok=True, parents=True)

    # Per-metric figures
    for metric_key, spec in METRIC_SPECS.items():
        out_path = out_dir / f"{args.prefix}_{spec['suffix']}.png"
        plot_metric_figure(
            structure_data,
            metric_key,
            out_path,
            overlay=args.overlay,
            color_map=color_map,
            marker_map=marker_map,
        )

    # Overview figure
    if args.overview:
        plot_overview(
            structure_data,
            out_dir / f"{args.prefix}_overview.png",
            overlay=args.overlay,
            color_map=color_map,
            marker_map=marker_map,
        )

    return 0


if __name__ == "__main__":
    main()
