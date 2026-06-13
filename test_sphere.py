"""
tests/test_sphere.py - Unit tests for Sphere ray intersection.

Run with: pytest tests/test_sphere.py -v
"""

import math
import pytest
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from src.vector import Vec3
from src.ray import Ray
from src.sphere import Sphere
from src.hittable import HitRecord


class TestSphereConstruction:
    def test_valid_sphere(self):
        s = Sphere(center=Vec3(0, 0, -1), radius=0.5)
        assert s.radius == 0.5

    def test_negative_radius_raises(self):
        with pytest.raises(ValueError):
            Sphere(center=Vec3(0, 0, 0), radius=-1.0)

    def test_zero_radius_raises(self):
        with pytest.raises(ValueError):
            Sphere(center=Vec3(0, 0, 0), radius=0.0)


class TestSphereHit:
    def _make_sphere(self) -> Sphere:
        """Half-unit sphere at (0, 0, -1): origin is outside the sphere."""
        return Sphere(center=Vec3(0.0, 0.0, -1.0), radius=0.5)

    def test_ray_hits_sphere_center(self):
        sphere = self._make_sphere()
        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, -1))
        record = HitRecord()
        hit = sphere.hit(ray, t_min=0.0, t_max=float("inf"), record=record)
        assert hit is True

    def test_hit_t_is_positive(self):
        sphere = self._make_sphere()
        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, -1))
        record = HitRecord()
        sphere.hit(ray, t_min=0.0, t_max=float("inf"), record=record)
        assert record.t > 0.0

    def test_hit_point_on_sphere_surface(self):
        """The hit point must lie on the sphere surface: |P - C| = r."""
        sphere = self._make_sphere()
        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, -1))
        record = HitRecord()
        sphere.hit(ray, t_min=0.0, t_max=float("inf"), record=record)
        dist = (record.point - sphere.center).length()
        assert dist == pytest.approx(sphere.radius, abs=1e-10)

    def test_ray_misses_sphere(self):
        sphere = self._make_sphere()
        # Ray shoots toward (2, 0, -1) — passes well to the right of the sphere
        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(2, 0, -1))
        record = HitRecord()
        hit = sphere.hit(ray, t_min=0.0, t_max=float("inf"), record=record)
        assert hit is False

    def test_ray_behind_sphere_misses(self):
        sphere = self._make_sphere()
        # Ray shoots in +Z, sphere is at Z=-1 (behind the origin)
        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, 1))
        record = HitRecord()
        hit = sphere.hit(ray, t_min=0.001, t_max=float("inf"), record=record)
        assert hit is False

    def test_normal_points_outward_on_front_face(self):
        sphere = self._make_sphere()
        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, -1))
        record = HitRecord()
        sphere.hit(ray, t_min=0.0, t_max=float("inf"), record=record)
        # Front face: normal should oppose the ray direction (dot < 0)
        assert ray.direction.dot(record.normal) < 0.0
        assert record.front_face is True

    def test_normal_is_unit_length(self):
        sphere = self._make_sphere()
        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, -1))
        record = HitRecord()
        sphere.hit(ray, t_min=0.0, t_max=float("inf"), record=record)
        assert record.normal.length() == pytest.approx(1.0, abs=1e-10)

    def test_t_min_clamps_close_hits(self):
        """Hits within t_min should be rejected (shadow acne prevention)."""
        sphere = self._make_sphere()
        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, -1))
        record = HitRecord()
        # First hit is at t ≈ 0 (front of sphere); reject it with large t_min
        hit = sphere.hit(ray, t_min=10.0, t_max=float("inf"), record=record)
        assert hit is False

    def test_t_max_clamps_far_hits(self):
        """Hits beyond t_max should be rejected."""
        sphere = self._make_sphere()
        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, -1))
        record = HitRecord()
        hit = sphere.hit(ray, t_min=0.0, t_max=0.4, record=record)
        assert hit is False

    def test_sphere_off_center_hit(self):
        """Ray grazing a sphere shifted off-axis."""
        sphere = Sphere(center=Vec3(2.0, 0.0, -5.0), radius=1.0)
        ray = Ray(origin=Vec3(2.0, 0.0, 0.0), direction=Vec3(0, 0, -1))
        record = HitRecord()
        hit = sphere.hit(ray, t_min=0.0, t_max=float("inf"), record=record)
        assert hit is True


class TestSceneIntegration:
    def test_scene_finds_closest_sphere(self):
        """Scene should return the closer of two overlapping spheres."""
        from src.scene import Scene
        scene = Scene()
        scene.add(Sphere(center=Vec3(0, 0, -2), radius=0.5))  # farther
        scene.add(Sphere(center=Vec3(0, 0, -1), radius=0.5))  # closer

        ray = Ray(origin=Vec3(0, 0, 0), direction=Vec3(0, 0, -1))
        record = HitRecord()
        hit = scene.hit(ray, t_min=0.001, t_max=float("inf"), record=record)

        assert hit is True
        # Closer sphere is at z=-1; hit t should be less than hitting z=-2 sphere
        assert record.t == pytest.approx(0.5, abs=0.01)