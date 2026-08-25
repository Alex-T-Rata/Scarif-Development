FROM php:8.5-apache

# Install and configure missing system database dependencies
RUN apt-get update && apt-get install -y \
    libpq-dev \
    && docker-php-ext-install pdo pdo_mysql \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Enforce system module configurations
RUN a2enmod rewrite

# Setup appropriate document permissions
WORKDIR /var/www/html