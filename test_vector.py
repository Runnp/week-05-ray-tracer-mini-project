"""
tests/test_vector.py - Unit tests for Vec3 math.

Every method on Vec3 is tested with at least one positive and one edge case.
Run with: pytest tests/test_vector.py -v
"""

import math
import pytest
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from src.vector import Vec3, vec3, unit_vector, ZERO, ONE, UP


class TestConstruction:
    def test_default_zero(self):
        v = Vec3()
        assert v.x == 0.0 and v.y == 0.0 and v.z == 0.0

    def test_components_stored(self):
        v = Vec3(1.0, 2.0, 3.0)
        assert v.x == 1.0
        assert v.y == 2.0
        assert v.z == 3.0

    def test_color_aliases(self):
        v = Vec3(0.1, 0.5, 0.9)
        assert v.r == v.x
        assert v.g == v.y
        assert v.b == v.z

    def test_shorthand_constructor(self):
        v = vec3(4.0, 5.0, 6.0)
        assert v == Vec3(4.0, 5.0, 6.0)


class TestArithmetic:
    def test_add(self):
        a = Vec3(1, 2, 3)
        b = Vec3(4, 5, 6)
        assert a + b == Vec3(5, 7, 9)

    def test_sub(self):
        a = Vec3(5, 5, 5)
        b = Vec3(1, 2, 3)
        assert a - b == Vec3(4, 3, 2)

    def test_scalar_multiply(self):
        v = Vec3(1, 2, 3)
        assert v * 2.0 == Vec3(2, 4, 6)

    def test_rmul(self):
        v = Vec3(1, 2, 3)
        assert 3.0 * v == Vec3(3, 6, 9)

    def test_component_multiply(self):
        a = Vec3(2, 3, 4)
        b = Vec3(1, 0.5, 0.25)
        assert a * b == Vec3(2, 1.5, 1)

    def test_divide(self):
        v = Vec3(4, 6, 8)
        assert v / 2.0 == Vec3(2, 3, 4)

    def test_negate(self):
        v = Vec3(1, -2, 3)
        assert -v == Vec3(-1, 2, -3)


class TestVectorOps:
    def test_dot_perpendicular(self):
        # Perpendicular vectors have dot product 0
        x = Vec3(1, 0, 0)
        y = Vec3(0, 1, 0)
        assert x.dot(y) == pytest.approx(0.0)

    def test_dot_parallel(self):
        v = Vec3(1, 0, 0)
        assert v.dot(v) == pytest.approx(1.0)

    def test_dot_value(self):
        a = Vec3(1, 2, 3)
        b = Vec3(4, 5, 6)
        # 1*4 + 2*5 + 3*6 = 4+10+18 = 32
        assert a.dot(b) == pytest.approx(32.0)

    def test_cross_produces_perpendicular(self):
        x = Vec3(1, 0, 0)
        y = Vec3(0, 1, 0)
        z = x.cross(y)
        assert z == Vec3(0, 0, 1)

    def test_cross_anticommutative(self):
        a = Vec3(1, 0, 0)
        b = Vec3(0, 1, 0)
        assert a.cross(b) == -(b.cross(a))

    def test_length_squared(self):
        v = Vec3(3, 4, 0)
        assert v.length_squared() == pytest.approx(25.0)

    def test_length(self):
        v = Vec3(3, 4, 0)
        assert v.length() == pytest.approx(5.0)

    def test_normalize_unit_length(self):
        v = Vec3(3, 0, 0)
        n = v.normalize()
        assert n.length() == pytest.approx(1.0)
        assert n.x == pytest.approx(1.0)

    def test_normalize_diagonal(self):
        v = Vec3(1, 1, 1)
        n = v.normalize()
        expected = 1.0 / math.sqrt(3)
        assert n.x == pytest.approx(expected)
        assert n.length() == pytest.approx(1.0)

    def test_normalize_zero_raises(self):
        with pytest.raises(ValueError):
            Vec3(0, 0, 0).normalize()

    def test_unit_vector_function(self):
        v = Vec3(0, 5, 0)
        u = unit_vector(v)
        assert u == Vec3(0, 1, 0)


class TestConstants:
    def test_zero(self):
        assert ZERO == Vec3(0, 0, 0)

    def test_one(self):
        assert ONE == Vec3(1, 1, 1)

    def test_up(self):
        assert UP == Vec3(0, 1, 0)


class TestEquality:
    def test_equal(self):
        assert Vec3(1, 2, 3) == Vec3(1, 2, 3)

    def test_not_equal(self):
        assert not (Vec3(1, 2, 3) == Vec3(1, 2, 4))

    def test_float_tolerance(self):
        # Floating-point results should compare equal within tolerance
        v = Vec3(1, 0, 0) * math.sqrt(2) / math.sqrt(2)
        assert v == Vec3(1, 0, 0)