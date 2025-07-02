import pandas as pd
import matplotlib.pyplot as plt

# Load your CSV
df = pd.read_csv("results.csv")

# Calculate d/ATT for each scheduling algorithm
for algo in ["FCFS", "SJF", "SRT", "PRI_RR"]:
    df[f"d/ATT_{algo}"] = df["d"] / df[algo]

# For each (n, k) configuration, create and save a separate plot
for (n_val, k_val), group in df.groupby(["n", "k"]):
    plt.figure()  # <--- starts a new figure
    for algo in ["FCFS", "SJF", "SRT", "PRI_RR"]:
        plt.plot(group["d"], group[f"d/ATT_{algo}"], label=algo, marker='o')
    plt.title(f"d/ATT over d for (n={n_val}, k={k_val})")
    plt.xlabel("d (mean CPU time)")
    plt.ylabel("d / ATT")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"d_over_att_n{n_val}_k{k_val}.png")  # Save to file
    plt.close()  # <--- ensures next plot starts fresh
