"""
tests/test_ray.py - Unit tests for the Ray class.

Run with: pytest tests/test_ray.py -v
"""

import pytest
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from src.vector import Vec3
from src.ray import Ray


class TestRayAt:
    def test_at_t0_equals_origin(self):
        r = Ray(origin=Vec3(1, 2, 3), direction=Vec3(0, 0, -1))
        assert r.at(0.0) == Vec3(1, 2, 3)

    def test_at_t1(self):
        r = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, -1))
        assert r.at(1.0) == Vec3(0, 0, -1)

    def test_at_t2(self):
        r = Ray(origin=Vec3(1, 0, 0), direction=Vec3(1, 0, 0))
        assert r.at(2.0) == Vec3(3, 0, 0)

    def test_at_negative_t_goes_backward(self):
        r = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, 1))
        assert r.at(-1.0) == Vec3(0, 0, -1)

    def test_at_half_t(self):
        r = Ray(origin=Vec3(0, 0, 0), direction=Vec3(2, 0, 0))
        assert r.at(0.5) == Vec3(1, 0, 0)