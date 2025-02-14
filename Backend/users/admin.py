from django.contrib import admin
from django.contrib.auth import get_user_model

User = get_user_model()

if User not in admin.site._registry:
    @admin.register(User)
    class CustomUserAdmin(admin.ModelAdmin):
        list_display = ("id", "username", "email")
        search_fields = ("username", "email")
else:
    pass
