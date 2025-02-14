# Use official Python image
FROM python:3.11

WORKDIR /app

# Copy requirements and install them
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy project files
COPY . .

# Expose port 8000
EXPOSE 8000

# Run migrations and start the server with Gunicorn
CMD ["sh", "-c", "python manage.py migrate && gunicorn Backend.wsgi:application --bind 0.0.0.0:8000"]
