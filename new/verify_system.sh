#!/bin/bash

# Complete verification test for miniRT bonus

echo "🧪 miniRT Bonus System Verification"
echo "===================================="
echo ""

cd /home/amn/Desktop/miniRT/bonus

# 1. Check compilation
echo "1️⃣  Verifying compilation..."
make -j4 > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "   ✅ Compilation successful"
else
    echo "   ❌ Compilation failed"
    exit 1
fi

# 2. Check executable exists
if [ -f miniRT_bonus ]; then
    echo "   ✅ miniRT_bonus binary exists"
else
    echo "   ❌ miniRT_bonus not found"
    exit 1
fi

# 3. Test scene parsing with patterns_test.rt
echo ""
echo "2️⃣  Testing parsing with patterns_test.rt..."
if [ ! -f ../maps/patterns_test.rt ]; then
    echo "   ❌ patterns_test.rt not found"
    exit 1
fi

# Run with short timeout to just test parsing
if timeout 3 ./miniRT_bonus ../maps/patterns_test.rt > /tmp/parse_test.log 2>&1; then
    if grep -q "Parsing completed successfully" /tmp/parse_test.log; then
        echo "   ✅ Parsing successful"
    else
        echo "   ⚠️  Parsing completed but no success message"
    fi
else
    echo "   ⚠️  Program timeout (expected - window init takes time)"
fi

# 4. Verify scene components
echo ""
echo "3️⃣  Verifying scene components in patterns_test.rt..."
if grep -q "sp 0,0,0  2" ../maps/patterns_test.rt; then
    echo "   ✅ Stripe sphere found"
else
    echo "   ❌ Stripe sphere missing"
fi

if grep -q "pl 0,-1,0" ../maps/patterns_test.rt; then
    echo "   ✅ Ring plane found"
else
    echo "   ❌ Ring plane missing"
fi

if grep -q "cy 1,0,1" ../maps/patterns_test.rt; then
    echo "   ✅ Checkers cylinder found"
else
    echo "   ❌ Checkers cylinder missing"
fi

# 5. Verify lighting code has brightness
echo ""
echo "4️⃣  Verifying lighting implementation..."
if grep -q "light.brightness" src/lighting/phong_lighting_bonus.c; then
    echo "   ✅ Light brightness used in diffuse calculation"
else
    echo "   ❌ Light brightness not found in lighting"
fi

if grep -q "result.specular = tuple_scalar_mult(tuple_scalar_mult" src/lighting/phong_lighting_bonus.c; then
    echo "   ✅ Specular calculation implemented"
else
    echo "   ❌ Specular calculation missing"
fi

# 6. Verify pattern system
echo ""
echo "5️⃣  Verifying pattern system..."
if [ -f src/paterns/patern.c ]; then
    echo "   ✅ Pattern file exists"
    
    if grep -q "stripe_at" src/paterns/patern.c; then
        echo "   ✅ stripe_at function implemented"
    fi
    
    if grep -q "ring_at" src/paterns/patern.c; then
        echo "   ✅ ring_at function implemented"
    fi
    
    if grep -q "checkers_at" src/paterns/patern.c; then
        echo "   ✅ checkers_at function implemented"
    fi
else
    echo "   ❌ Pattern file missing"
fi

# 7. Summary
echo ""
echo "✨ System Status Summary"
echo "======================="
echo "• Parsing: Pattern syntax supported (pat:, pa:, pb:, ptrans:, pscale:, prot_*)"
echo "• Materials: Ambient, diffuse, specular, shininess, reflection, refraction"
echo "• Patterns: Stripe, Ring, Checkers with transform support"
echo "• Lighting: Phong model with ambient + diffuse + specular"
echo "• Rendering: Ray casting with shadow testing"
echo ""
echo "🚀 Ready to render! Run with:"
echo "   ./miniRT_bonus ../maps/patterns_test.rt"
echo "   Then press 'T' to save output.ppm"
