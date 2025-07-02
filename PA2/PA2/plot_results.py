import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results.tsv', sep='\t', header=None,
                 names=["n", "k", "d", "v", "q", "FCFS", "SJF", "SRT", "PRI_RR"])

for algo in ["FCFS", "SJF", "SRT", "PRI_RR"]:
    df[f"d/ATT_{algo}"] = df["d"] / df[algo]

for (n_val, k_val), group in df.groupby(["n", "k"]):
    plt.figure()
    for algo in ["FCFS", "SJF", "SRT", "PRI_RR"]:
        plt.plot(group["d"], group[f"d/ATT_{algo}"], label=algo, marker='o')
    plt.title(f"d/ATT over d for (n={n_val}, k={k_val})")
    plt.xlabel("d (mean CPU time)")
    plt.ylabel("d / ATT")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"d_over_att_n{n_val}_k{k_val}.png")
    plt.show()
