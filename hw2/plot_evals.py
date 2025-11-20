import csv
from pathlib import Path

import matplotlib.pyplot as plt

ROOT_DIR = Path(__file__).parent
EVALS_DIR = ROOT_DIR / "evals"


def read_csv_dicts(path: Path):
    with path.open(newline="") as f:
        reader = csv.DictReader(f)
        return list(reader)


def plot_fig1_insert_time():
    data = read_csv_dicts(EVALS_DIR / "fig1_insert_time.csv")

    n = [int(row["n"]) for row in data]
    bst = [float(row["BST_us_per_insert"]) for row in data]
    avl = [float(row["AVL_us_per_insert"]) for row in data]
    treap = [float(row["Treap_us_per_insert"]) for row in data]
    skip_p05 = [float(row["SkipList_p0.5_us_per_insert"]) for row in data]

    plt.figure()
    plt.plot(n, bst, marker="o", label="BST")
    plt.plot(n, avl, marker="s", label="AVL")
    plt.plot(n, treap, marker="^", label="Treap")
    plt.plot(n, skip_p05, marker="D", label="Skip List (p=0.5)")

    plt.xscale("log", base=2)
    plt.xlabel("n")
    plt.ylabel("Average insert time (µs)")
    plt.title("Figure 1: Insert time vs n")
    plt.grid(True, which="both", linestyle="--", alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(EVALS_DIR / "fig1_insert_time.png", dpi=300)
    plt.close()


def plot_fig2_search_time():
    data = read_csv_dicts(EVALS_DIR / "fig2_search_time.csv")

    n = [int(row["n"]) for row in data]
    bst = [float(row["BST_us_per_search"]) for row in data]
    avl = [float(row["AVL_us_per_search"]) for row in data]
    treap = [float(row["Treap_us_per_search"]) for row in data]
    skip_p05 = [float(row["SkipList_p0.5_us_per_search"]) for row in data]

    plt.figure()
    plt.plot(n, bst, marker="o", label="BST")
    plt.plot(n, avl, marker="s", label="AVL")
    plt.plot(n, treap, marker="^", label="Treap")
    plt.plot(n, skip_p05, marker="D", label="Skip List (p=0.5)")

    plt.xscale("log", base=2)
    plt.xlabel("n")
    plt.ylabel("Average search time (µs)")
    plt.title("Figure 2: Search time vs n")
    plt.grid(True, which="both", linestyle="--", alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(EVALS_DIR / "fig2_search_time.png", dpi=300)
    plt.close()


def plot_fig3_height():
    data = read_csv_dicts(EVALS_DIR / "fig3_height.csv")

    n = [int(row["n"]) for row in data]

    avl = [float(row["AVL_height"]) for row in data]
    treap = [float(row["Treap_height"]) for row in data]
    skip_p05 = [float(row["SkipList_p0.5_height"]) for row in data]
    skip_p075 = [float(row["SkipList_p0.75_height"]) for row in data]
    skip_p025 = [float(row["SkipList_p0.25_height"]) for row in data]
    avl_bf3 = [float(row["AVL_BF3_height"]) for row in data]

    bst = [float(row["BST_height"]) for row in data]

    plt.figure()
    plt.plot(n, bst, marker="o", label="BST")
    plt.plot(n, avl, marker="s", label="AVL")
    plt.plot(n, treap, marker="^", label="Treap")
    plt.plot(n, skip_p05, marker="D", label="Skip List (p=0.5)")
    plt.plot(n, skip_p075, marker="v", label="Skip List (p=0.75)")
    plt.plot(n, skip_p025, marker="P", label="Skip List (p=0.25)")
    plt.plot(n, avl_bf3, marker="X", label="AVL (|BF| ≤ 3)")

    plt.xscale("log", base=2)
    plt.xlabel("n")
    plt.ylabel("Average height")
    plt.title("Figure 3: Height vs n")
    plt.grid(True, which="both", linestyle="--", alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(EVALS_DIR / "fig3_height.png", dpi=300)
    plt.close()


def plot_fig3_height_no_bst():
    data = read_csv_dicts(EVALS_DIR / "fig3_height.csv")

    n = [int(row["n"]) for row in data]

    avl = [float(row["AVL_height"]) for row in data]
    treap = [float(row["Treap_height"]) for row in data]
    skip_p05 = [float(row["SkipList_p0.5_height"]) for row in data]
    skip_p075 = [float(row["SkipList_p0.75_height"]) for row in data]
    skip_p025 = [float(row["SkipList_p0.25_height"]) for row in data]
    avl_bf3 = [float(row["AVL_BF3_height"]) for row in data]

    plt.figure()
    plt.plot(n, avl, marker="s", label="AVL")
    plt.plot(n, treap, marker="^", label="Treap")
    plt.plot(n, skip_p05, marker="D", label="Skip List (p=0.5)")
    plt.plot(n, skip_p075, marker="v", label="Skip List (p=0.75)")
    plt.plot(n, skip_p025, marker="P", label="Skip List (p=0.25)")
    plt.plot(n, avl_bf3, marker="X", label="AVL (|BF| ≤ 3)")

    plt.xscale("log", base=2)
    plt.xlabel("n")
    plt.ylabel("Average height")
    plt.title("Figure 3 (without BST): Height vs n")
    plt.grid(True, which="both", linestyle="--", alpha=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(EVALS_DIR / "fig3_height_no_bst.png", dpi=300)
    plt.close()


def main():
    plot_fig1_insert_time()
    plot_fig2_search_time()
    plot_fig3_height()
    plot_fig3_height_no_bst()


if __name__ == "__main__":
    main()
