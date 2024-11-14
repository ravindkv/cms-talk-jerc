import json
import matplotlib.pyplot as plt

# Load the JSON file
with open('jetvetomaps_2018.json', 'r') as f:
    data = json.load(f)

# Access the content
content = data["corrections"][0]['data']['content']

# Create a single plot for overlaying all histograms
plt.figure()
for index in range(5):
    key = content[index]['key']
    if "jetvetomap_all" in key: continue
    values = content[index]['value']['content']

    # Plot histogram if the list has values
    if values:
        plt.hist(values, bins=10, histtype='step', linewidth=1.5, label=f'Key: {key}')

plt.title('Overlaid Histograms for All Keys')
plt.xlabel('Value')
plt.ylabel('Frequency')
plt.legend()
plt.grid(axis='y', alpha=0.75)
plt.savefig('vetoMaps_2018UL.pdf', format='pdf', dpi=300)
plt.show()

