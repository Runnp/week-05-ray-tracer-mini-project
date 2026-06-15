# week-05-ray-tracer-mini-project

A CPU ray tracer built from scratch in pure Python, following the spirit of
Peter Shirley's *Ray Tracing in One Weekend* — organized as a professional
software project.

**Stack:** Python · NumPy · Pillow · pytest

---

## Setup

```bash
pip install -r requirements.txt
```

## Render Phase 1

```bash
python src/main.py
# Output → renders/phase1_red_sphere.png
```

## Run Tests

```bash
pytest tests/ -v
```

---

## Project Structure

```
src/
  main.cpp     
  ray.h       
  vec3.h
  
renders/       — Output images (git-ignored)
```

---

## Phases

| Phase | Feature                        
|-------|--------------------------------
| 1     | Red sphere, sky background     
| 2     | Surface normals, multi-sphere  
| 3     | Lambertian diffuse             
| 4     | Metal reflections              
| 5     | Dielectric glass               
| 6     | Anti-aliasing, gamma           
| 7     | Depth of field, camera control 
| 8     | BVH acceleration               
| 9     | Multithreaded rendering        