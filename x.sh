#!/bin/bash
# This script creates the file structure for a Django backend project
# with Users (JWT auth), user settings, Docker containerization, and a locally hosted database.

# Root directory for the project (change "Backend" if you want a different name)
PROJECT_ROOT="Backend"

# Create root project directories
mkdir -p "$PROJECT_ROOT"/{requirements,Backend,users,user_settings,tests}

# Create subdirectories for settings and migrations
mkdir -p "$PROJECT_ROOT"/Backend/settings
mkdir -p "$PROJECT_ROOT"/users/migrations
mkdir -p "$PROJECT_ROOT"/user_settings/migrations

# Create root-level files
touch "$PROJECT_ROOT"/Dockerfile
touch "$PROJECT_ROOT"/docker-compose.yml
touch "$PROJECT_ROOT"/.env
touch "$PROJECT_ROOT"/README.md
touch "$PROJECT_ROOT"/manage.py

# Create requirements files
touch "$PROJECT_ROOT"/requirements/{base.txt,development.txt,production.txt}

# Create Django project package files
touch "$PROJECT_ROOT"/Backend/__init__.py
touch "$PROJECT_ROOT"/Backend/settings/{__init__.py,base.py,development.py,production.py}
touch "$PROJECT_ROOT"/Backend/urls.py
touch "$PROJECT_ROOT"/Backend/wsgi.py
touch "$PROJECT_ROOT"/Backend/asgi.py

# Create Users app files
touch "$PROJECT_ROOT"/users/{__init__.py,admin.py,apps.py,models.py,serializers.py,urls.py,views.py,permissions.py}
touch "$PROJECT_ROOT"/users/migrations/__init__.py

# Create User Settings app files
touch "$PROJECT_ROOT"/user_settings/{__init__.py,admin.py,apps.py,models.py,serializers.py,urls.py,views.py}
touch "$PROJECT_ROOT"/user_settings/migrations/__init__.py

# Create tests files
touch "$PROJECT_ROOT"/tests/{__init__.py,test_endpoints.py}

echo "Project structure created successfully."
