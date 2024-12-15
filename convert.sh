mkdir -p src/ui

for file in src/forms/*.ui; do
    filename=$(basename "$file" .ui)
    /opt/local/libexec/qt6/libexec/uic "$file" -o "src/ui/ui_${filename}.h"
done

echo "All files converted."