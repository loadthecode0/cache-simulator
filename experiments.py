import subprocess
import pandas as pd
import matplotlib.pyplot as plt

# Define the ranges or lists of parameters to test
n_sets_values = [2, 4, 16, 256]
n_blocks_values = [2, 4, 16, 256]
n_bytes_per_block_values = [4, 16, 256]
wa_values = ["write-allocate", "no-write-allocate"]
wt_values = ["write-through", "write-back"]
evictions = ["lru", "fifo"]

# List to store the results
results = []

# Run the executable with different parameters
count = 0
# for p1 in n_sets_values:
for p2 in n_blocks_values:
    for wa in wa_values:
        for wt in wt_values:
            for ev in evictions:
                for p3 in n_bytes_per_block_values:
                    p1 = 1
                    with open("gcc.trace", 'r') as f:
                        count += 1
                        process = subprocess.run(['./cacheSim', str(p1), str(p2), str(p3), wa, wt, ev], stdin=f, capture_output=True, text=True)
                        output = process.stdout.strip()
                        # print(f"Running program with params ({p1}, {p2}, {16}, {wa}, {wt}, {ev}) ->\n {output}")
                        # Assume output format is: n_sets,n_blocks,result
                        parts = output.split()
                        results.append({
                            'n_sets': p1,
                            'n_blocks': p2,
                            'bytes_per_block': p3,
                            'wa': wa,
                            'wt': wt,
                            'ev': ev,
                            'total_loads': (parts[2]),
                            'total_stores': (parts[5]),
                            'load_hits': (parts[8]),
                            'load_misses': (parts[11]),
                            'store_hits': (parts[14]),
                            'store_misses': (parts[17]),
                            'total_cycles': (parts[20])
                        })
                        print(count)
                            # print(results)

# Convert results to DataFrame
df = pd.DataFrame(results)

# Save results to CSV (optional)
df.to_csv('results_fully_assoc.csv', index=False)

print(df)
