import sys
from vedo import *

param1 = sys.argv[1]

# Load the STL file
kunai = load(param1)

# Initialize a custom 3D plotter
plt = Plotter(offscreen=True)

plt.look_at('yz')

plt.azimuth(45)

plt.elevation(45)

# Show the STL file in the plotter
plt.show(kunai, interactive=False)

param1= param1.replace("./STL_Files/", "./Image/").replace(".stl", ".png")
# Save the screenshot
plt.screenshot(param1)

# Set the plotter to interactive mode and then close it
plt.close()

# Process parameters (replace this with your logic)
result = f"{param1}"

# Print the result to be captured by shell_exec in PHP
print(result)
