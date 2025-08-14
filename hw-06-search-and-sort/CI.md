# Continuous Integration (CI) Setup

## supported versions for GUI libs

### imgui version: 1.91.1

Remove-Item -Recurse -Force external\imgui

#### Clone the imgui repository with a specific version

git clone --branch v1.91.1 --depth 1 <https://github.com/ocornut/imgui.git> external/imgui

### imgui-sfml version: 3.0.0
