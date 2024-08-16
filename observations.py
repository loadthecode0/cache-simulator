import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

df = pd.read_csv('results.csv')
df2 = pd.read_csv('results_direct.csv')
df3 = pd.read_csv('results_fully_assoc.csv')

df = pd.concat([df, df2, df3], axis=0)

# Calculate hit and miss rates
df['Total Hits'] = df['load_hits'] + df['store_hits']
df['Total Misses'] = df['load_misses'] + df['store_misses']
df['Total Accesses'] = df['total_loads'] + df['total_stores']

df['Hit_Rate'] = df['Total Hits'] / df['Total Accesses']
df['Miss_Rate'] = df['Total Misses'] / df['Total Accesses']

# Calculate total cache size including overhead
# This is a simple model; adjust the overhead calculation as per your cache's metadata
bits_per_byte = 8
validity_bits_per_block = 1  # Change if different in your case
# Estimate the number of tag bits: Log base 2 of the n_sets (assuming fully associative)
df['Tag Bits'] = (32 - df['n_sets'].apply(lambda x: (x.bit_length() - 1) if x > 0 else 0) - df['bytes_per_block'].apply(lambda x: (x.bit_length() - 1) if x > 0 else 0))
df['Overhead per Block'] = (df['Tag Bits'] + validity_bits_per_block) / bits_per_byte  # Convert bits to bytes

# Total cache size = (bytes/block + overhead/block) * blocks/set * sets
df['Total_Cache_Size_(kibibytes)'] = ((df['bytes_per_block'] + df['Overhead per Block']) * df['n_blocks'] * df['n_sets']/1024).astype('int')

bins = [0, 4, 16, 64, 256, 1024, 4096, 10000, 17000 ]

# Use cut to categorize the data
df['Cache_Size_(KiB)'] = pd.cut(df['Total_Cache_Size_(kibibytes)'], bins=bins, labels=["<4", "4-16", "16-64","64-256", "256-1024", "1024-4096", "4096-10000", ">10000" ])

# Display the DataFrame with new columns
# print(df[['Hit Rate', 'Miss Rate', 'Total Cache Size (bytes)']])


print(df.columns)
# print(max(df['Total_Cache_Size_(kibibytes)']))

# df = df[(df['n_blocks'] * df['n_sets'] == 1024)]
print(df)

xdataname = 'n_blocks'
ydataname = 'Miss_Rate'
groupby = 'wt'
title = 'Associativity vs Miss Rate for Different Write Policies'


dict = {'n_sets' : 'Cache Sets',
        'n_blocks' : 'Associativity (Blocks per set)',        
        'bytes_per_block' : 'Block size (bytes)',
        'wa' : 'Write-Allocate policy',
        'wt' : 'Write policy',
        'ev' : 'Eviction policy',
        'total_cycles' : 'Total Cycles',
        'Hit_Rate' : 'Hit Rate',
        'Miss_Rate' : 'Miss Rate', 
        # 'Tag_Bits' : 'Tag Bits',
        # 'Overhead per Block' : 'Overhead per Block', 
        'Total_Cache_Size_(kibibytes)' : 'Total Cache Size (kibibytes)',
        'Cache_Size_(KiB)' : 'Cache Size (KiB)'
        }


xlabel = dict[xdataname]
ylabel = dict[ydataname]
figname = xdataname + '_vs_'+ ydataname + '_grpby_ ' + groupby +'_eg2'

# Plot results
# plt.figure(figsize=(10, 5))


# # Plotting
# for key, grp in df.groupby(groupby):
#     plt.scatter(grp[xdataname], grp[ydataname], marker='o', label=f'n_sets={key}')

# # Setting x-axis to a logarithmic scale
# # plt.xscale('log')

# # Defining x-tick positions
# # The positions are determined based on your data range and spacing needs
# # xtick_positions = [2, 4, 16, 256]

# # Setting custom x-ticks
# # plt.xticks(xtick_positions, labels=[str(x) for x in xtick_positions])

# plt.xlabel(xlabel)
# plt.ylabel(ylabel)
# plt.title(title)
# handles, labels = plt.gca().get_legend_handles_labels()
# new_labels = [label.replace('n_sets=', dict[groupby]+': ') for label in labels]
# plt.legend(handles, new_labels)
# plt.grid(True)


df = df[(df['bytes_per_block'] == 16)]
print(df)
df_aggregated = df.groupby(['n_sets', 'n_blocks']).agg({'total_cycles': 'mean'}).reset_index()
pivot_table = df_aggregated.pivot(index="n_sets", columns="n_blocks", values="total_cycles")

# Creating the heatmap
plt.figure(figsize=(10, 7))
plt.xlabel("Number of Blocks")
plt.ylabel("Number of Sets")
sns.heatmap(pivot_table, annot=True, cmap='coolwarm', fmt=".3f")
plt.title('Number of Sets vs Number of Blocks vs Total Cycles for 4-word blocks')
# plt.show()

# Save the figure
plt.savefig('hm2' + '.png')