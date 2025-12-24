#!/bin/bash
# Quick PPM analysis script

if [ ! -f "output.ppm" ]; then
    echo "❌ output.ppm not found in current directory"
    exit 1
fi

SIZE=$(ls -lh output.ppm | awk '{print $5}')
PIXELS=$(tail -n +4 output.ppm | tr -s ' ' '\n' | grep -v '^$' | wc -l)
RGB_COUNT=$((PIXELS / 3))

echo "📊 PPM File Analysis"
echo "===================="
echo "File size: $SIZE"
echo "Total RGB values: $PIXELS"
echo "Approximate pixels: $RGB_COUNT"
echo ""

# Get unique values
UNIQUE=$(tail -n +4 output.ppm | tr -s ' ' '\n' | sort -u | grep -v '^$' | head -20)
echo "Sample unique values (first 20):"
echo "$UNIQUE"
echo ""

# Check for colored vs black
BLACK_COUNT=$(tail -n +4 output.ppm | tr -s ' ' '\n' | grep '^0$' | wc -l)
COLORED=$(tail -n +4 output.ppm | tr -s ' ' '\n' | grep -v '^0$' | grep -v '^$' | wc -l)

echo "Pixel statistics:"
echo "  Black (0) values: $BLACK_COUNT"
echo "  Colored (non-zero) values: $COLORED"
echo "  Ratio colored: $(( (COLORED * 100) / (COLORED + BLACK_COUNT) ))%"
echo ""

# Check if patterns are visible (should have values between 1-254)
MIN_VAL=$(tail -n +4 output.ppm | tr -s ' ' '\n' | grep -v '^$' | sort -n | head -1)
MAX_VAL=$(tail -n +4 output.ppm | tr -s ' ' '\n' | grep -v '^$' | sort -n | tail -1)

echo "Value range: $MIN_VAL to $MAX_VAL"
echo ""
echo "✅ If MAX_VAL is close to 255 and you see varied values, patterns and lighting are working!"
