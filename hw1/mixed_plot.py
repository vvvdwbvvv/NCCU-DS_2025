import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
from matplotlib.ticker import LogFormatter

# Define fonts and style - match plot.py
plt.style.use("seaborn-v0_8-paper")
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

# Match the color scheme from plot.py
STRUCTURE_COLORS = {
    "DS1": "#418af7",  # Blue
    "DS2": "#5fb075",  # Green
    "DS3": "#ee613d",  # Red
}

STRUCTURE_MARKERS = {
    "DS1": "o",
    "DS2": "v",
    "DS3": "s",
}

def load_data(csv_path: Path) -> pd.DataFrame:
    """Load the mixed operations results CSV file."""
    df = pd.read_csv(csv_path)
    return df

def plot_throughput_comparison(df: pd.DataFrame, output_dir: Path):
    """Create a grouped bar chart comparing throughput across workloads."""
    fig, ax = plt.subplots(figsize=(12, 7))
    
    workloads = df['Workload'].unique()
    structures = df['Type'].unique()
    
    x = np.arange(len(workloads))
    width = 0.25
    
    for i, struct in enumerate(structures):
        data = df[df['Type'] == struct]
        throughput = data['Throughput_ops_per_sec'].values
        offset = width * (i - 1)
        bars = ax.bar(x + offset, throughput, width, 
                     label=struct, 
                     color=STRUCTURE_COLORS[struct],
                     edgecolor='black',
                     linewidth=1.5)
    
    ax.set_xlabel('Workload Type')
    ax.set_ylabel('Throughput (ops/sec)')
    ax.set_title('Throughput Comparison Across Workloads')
    ax.set_xticks(x)
    ax.set_xticklabels(workloads, rotation=15, ha='right')
    ax.set_yscale('log')
    
    # Use LogFormatter for better readability
    formatter = LogFormatter(10, labelOnlyBase=False, minor_thresholds=(2, 0.4))
    ax.get_yaxis().set_major_formatter(formatter)
    
    ax.legend(frameon=False)
    ax.grid(True, which='major', linestyle='-', linewidth=0.7, alpha=0.5)
    ax.minorticks_on()
    ax.grid(True, which='minor', linestyle=':', linewidth=0.5, alpha=0.3)
    ax.set_axisbelow(True)
    
    fig.tight_layout(pad=1.5)
    fig.savefig(output_dir / "throughput_comparison.png", dpi=160)
    fig.savefig(output_dir / "throughput_comparison.pdf")
    plt.close(fig)
    print(f"Saved: {output_dir / 'throughput_comparison.png'}")

def plot_avg_operation_time(df: pd.DataFrame, output_dir: Path):
    """Create a grouped bar chart comparing average operation time."""
    fig, ax = plt.subplots(figsize=(12, 7))
    
    workloads = df['Workload'].unique()
    structures = df['Type'].unique()
    
    x = np.arange(len(workloads))
    width = 0.25
    
    for i, struct in enumerate(structures):
        data = df[df['Type'] == struct]
        avg_time = data['AvgOpTime_us'].values
        offset = width * (i - 1)
        bars = ax.bar(x + offset, avg_time, width, 
                     label=struct, 
                     color=STRUCTURE_COLORS[struct],
                     edgecolor='black',
                     linewidth=1.5)
    
    ax.set_xlabel('Workload Type')
    ax.set_ylabel('Average Operation Time (μs)')
    ax.set_title('Average Operation Time Across Workloads')
    ax.set_xticks(x)
    ax.set_xticklabels(workloads, rotation=15, ha='right')
    ax.set_yscale('log')
    
    # Use LogFormatter for better readability
    formatter = LogFormatter(10, labelOnlyBase=False, minor_thresholds=(2, 0.4))
    ax.get_yaxis().set_major_formatter(formatter)
    
    ax.legend(frameon=False)
    ax.grid(True, which='major', linestyle='-', linewidth=0.7, alpha=0.5)
    ax.minorticks_on()
    ax.grid(True, which='minor', linestyle=':', linewidth=0.5, alpha=0.3)
    ax.set_axisbelow(True)
    
    fig.tight_layout(pad=1.5)
    fig.savefig(output_dir / "avg_operation_time.png", dpi=160)
    fig.savefig(output_dir / "avg_operation_time.pdf")
    plt.close(fig)
    print(f"Saved: {output_dir / 'avg_operation_time.png'}")

def plot_total_time(df: pd.DataFrame, output_dir: Path):
    """Create a grouped bar chart comparing total execution time."""
    fig, ax = plt.subplots(figsize=(12, 7))
    
    workloads = df['Workload'].unique()
    structures = df['Type'].unique()
    
    x = np.arange(len(workloads))
    width = 0.25
    
    for i, struct in enumerate(structures):
        data = df[df['Type'] == struct]
        total_time = data['TotalTime'].values
        offset = width * (i - 1)
        bars = ax.bar(x + offset, total_time, width, 
                     label=struct, 
                     color=STRUCTURE_COLORS[struct],
                     edgecolor='black',
                     linewidth=1.5)
    
    ax.set_xlabel('Workload Type')
    ax.set_ylabel('Total Execution Time (s)')
    ax.set_title('Total Execution Time Across Workloads')
    ax.set_xticks(x)
    ax.set_xticklabels(workloads, rotation=15, ha='right')
    ax.set_yscale('log')
    
    # Use LogFormatter for better readability
    formatter = LogFormatter(10, labelOnlyBase=False, minor_thresholds=(2, 0.4))
    ax.get_yaxis().set_major_formatter(formatter)
    
    ax.legend(frameon=False)
    ax.grid(True, which='major', linestyle='-', linewidth=0.7, alpha=0.5)
    ax.minorticks_on()
    ax.grid(True, which='minor', linestyle=':', linewidth=0.5, alpha=0.3)
    ax.set_axisbelow(True)
    
    fig.tight_layout(pad=1.5)
    fig.savefig(output_dir / "total_time.png", dpi=160)
    fig.savefig(output_dir / "total_time.pdf")
    plt.close(fig)
    print(f"Saved: {output_dir / 'total_time.png'}")

def plot_heatmap(df: pd.DataFrame, output_dir: Path, metric: str):
    """Create a heatmap showing performance across workloads and structures."""
    fig, ax = plt.subplots(figsize=(12, 7))
    
    # Pivot the data for heatmap
    pivot_data = df.pivot(index='Type', columns='Workload', values=metric)
    
    # Use log scale for better visualization
    pivot_data_log = np.log10(pivot_data)
    
    im = ax.imshow(pivot_data_log, cmap='YlOrRd', aspect='auto')
    
    # Set ticks and labels
    ax.set_xticks(np.arange(len(pivot_data.columns)))
    ax.set_yticks(np.arange(len(pivot_data.index)))
    ax.set_xticklabels(pivot_data.columns, rotation=15, ha='right')
    ax.set_yticklabels(pivot_data.index)
    
    # Add colorbar
    cbar = plt.colorbar(im, ax=ax)
    if metric == 'Throughput_ops_per_sec':
        cbar.set_label('log₁₀(Throughput ops/sec)', rotation=270, labelpad=20)
        title = 'Throughput Heatmap (log₁₀ scale)'
    elif metric == 'AvgOpTime_us':
        cbar.set_label('log₁₀(Avg Time μs)', rotation=270, labelpad=20)
        title = 'Average Operation Time Heatmap (log₁₀ scale)'
    else:
        cbar.set_label('log₁₀(Total Time s)', rotation=270, labelpad=20)
        title = 'Total Time Heatmap (log₁₀ scale)'
    
    # Add text annotations
    for i in range(len(pivot_data.index)):
        for j in range(len(pivot_data.columns)):
            value = pivot_data.iloc[i, j]
            text = ax.text(j, i, f'{value:.2e}' if value > 1000 else f'{value:.2f}',
                         ha="center", va="center", color="black", fontsize=10,
                         weight='bold')
    
    ax.set_title(title)
    fig.tight_layout(pad=1.5)
    
    filename = metric.lower().replace('_', '_')
    fig.savefig(output_dir / f"heatmap_{filename}.png", dpi=160)
    fig.savefig(output_dir / f"heatmap_{filename}.pdf")
    plt.close(fig)
    print(f"Saved: {output_dir / f'heatmap_{filename}.png'}")

def plot_overview(df: pd.DataFrame, output_dir: Path):
    """Create an overview figure with multiple subplots."""
    fig, axes = plt.subplots(2, 2, figsize=(18, 13))
    
    workloads = df['Workload'].unique()
    structures = df['Type'].unique()
    x = np.arange(len(workloads))
    width = 0.25
    
    # Plot 1: Throughput
    ax = axes[0, 0]
    for i, struct in enumerate(structures):
        data = df[df['Type'] == struct]
        throughput = data['Throughput_ops_per_sec'].values
        offset = width * (i - 1)
        ax.bar(x + offset, throughput, width, label=struct, 
               color=STRUCTURE_COLORS[struct], edgecolor='black', linewidth=1.5)
    ax.set_ylabel('Throughput (ops/sec)')
    ax.set_title('Throughput Comparison')
    ax.set_xticks(x)
    ax.set_xticklabels(workloads, rotation=15, ha='right')
    ax.set_yscale('log')
    formatter = LogFormatter(10, labelOnlyBase=False, minor_thresholds=(2, 0.4))
    ax.get_yaxis().set_major_formatter(formatter)
    ax.legend(frameon=False)
    ax.grid(True, which='major', linestyle='-', linewidth=0.7, alpha=0.5)
    ax.minorticks_on()
    ax.grid(True, which='minor', linestyle=':', linewidth=0.5, alpha=0.3)
    ax.set_axisbelow(True)
    
    # Plot 2: Avg Operation Time
    ax = axes[0, 1]
    for i, struct in enumerate(structures):
        data = df[df['Type'] == struct]
        avg_time = data['AvgOpTime_us'].values
        offset = width * (i - 1)
        ax.bar(x + offset, avg_time, width, label=struct, 
               color=STRUCTURE_COLORS[struct], edgecolor='black', linewidth=1.5)
    ax.set_ylabel('Average Operation Time (μs)')
    ax.set_title('Average Operation Time')
    ax.set_xticks(x)
    ax.set_xticklabels(workloads, rotation=15, ha='right')
    ax.set_yscale('log')
    formatter = LogFormatter(10, labelOnlyBase=False, minor_thresholds=(2, 0.4))
    ax.get_yaxis().set_major_formatter(formatter)
    ax.legend(frameon=False)
    ax.grid(True, which='major', linestyle='-', linewidth=0.7, alpha=0.5)
    ax.minorticks_on()
    ax.grid(True, which='minor', linestyle=':', linewidth=0.5, alpha=0.3)
    ax.set_axisbelow(True)
    
    # Plot 3: Total Time
    ax = axes[1, 0]
    for i, struct in enumerate(structures):
        data = df[df['Type'] == struct]
        total_time = data['TotalTime'].values
        offset = width * (i - 1)
        ax.bar(x + offset, total_time, width, label=struct, 
               color=STRUCTURE_COLORS[struct], edgecolor='black', linewidth=1.5)
    ax.set_xlabel('Workload Type')
    ax.set_ylabel('Total Execution Time (s)')
    ax.set_title('Total Execution Time')
    ax.set_xticks(x)
    ax.set_xticklabels(workloads, rotation=15, ha='right')
    ax.set_yscale('log')
    formatter = LogFormatter(10, labelOnlyBase=False, minor_thresholds=(2, 0.4))
    ax.get_yaxis().set_major_formatter(formatter)
    ax.legend(frameon=False)
    ax.grid(True, which='major', linestyle='-', linewidth=0.7, alpha=0.5)
    ax.minorticks_on()
    ax.grid(True, which='minor', linestyle=':', linewidth=0.5, alpha=0.3)
    ax.set_axisbelow(True)
    
    # Plot 4: Performance Ranking
    ax = axes[1, 1]
    for struct in structures:
        data = df[df['Type'] == struct]
        rankings = []
        for workload in workloads:
            workload_data = df[df['Workload'] == workload]
            sorted_data = workload_data.sort_values('Throughput_ops_per_sec', ascending=False)
            rank = list(sorted_data['Type']).index(struct) + 1
            rankings.append(rank)
        ax.plot(x, rankings, marker=STRUCTURE_MARKERS[struct], markersize=10, 
               linewidth=2.0, label=struct, color=STRUCTURE_COLORS[struct],
               markeredgewidth=1.5, markeredgecolor='black')
    ax.set_xlabel('Workload Type')
    ax.set_ylabel('Rank (by Throughput)')
    ax.set_title('Performance Ranking')
    ax.set_xticks(x)
    ax.set_xticklabels(workloads, rotation=15, ha='right')
    ax.set_yticks([1, 2, 3])
    ax.set_ylim(0.5, 3.5)
    ax.invert_yaxis()
    ax.legend(frameon=False)
    ax.grid(True, which='major', linestyle='-', linewidth=0.7, alpha=0.5)
    ax.minorticks_on()
    ax.grid(True, which='minor', linestyle=':', linewidth=0.5, alpha=0.3)
    ax.set_axisbelow(True)
    
    fig.suptitle('Mixed Operations Performance Overview', fontsize=18, y=0.995)
    fig.tight_layout(rect=[0, 0, 1, 0.99])
    fig.savefig(output_dir / "overview.png", dpi=160)
    fig.savefig(output_dir / "overview.pdf")
    plt.close(fig)
    print(f"Saved: {output_dir / 'overview.png'}")

def main():
    # Set up paths
    csv_path = Path("mixed_ops_results.csv")
    output_dir = Path("mix_plots")
    output_dir.mkdir(exist_ok=True)
    
    # Load data
    print(f"Loading data from {csv_path}...")
    df = load_data(csv_path)
    
    # Generate plots
    print("\nGenerating plots...")
    plot_throughput_comparison(df, output_dir)
    plot_avg_operation_time(df, output_dir)
    plot_total_time(df, output_dir)
    plot_heatmap(df, output_dir, 'Throughput_ops_per_sec')
    plot_heatmap(df, output_dir, 'AvgOpTime_us')
    plot_heatmap(df, output_dir, 'TotalTime')
    plot_overview(df, output_dir)
    
    print(f"\nAll plots saved to {output_dir}/")

if __name__ == "__main__":
    main()