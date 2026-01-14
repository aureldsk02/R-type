#!/usr/bin/env bash

# Fichier contenant les variables (par ex. vars.txt)
CONFIG_FILE="../Secrets/.secret_key.txt"

# Vérifier si le fichier existe
if [[ ! -f "$CONFIG_FILE" ]]; then
    echo "Le fichier $CONFIG_FILE est introuvable."
    exit 1
fi

# Détection du système
OS_TYPE="$(uname | tr '[:upper:]' '[:lower:]')"

if [[ "$OS_TYPE" == *"linux"* || "$OS_TYPE" == *"darwin"* ]]; then
    SYSTEM="linux"
elif [[ "$OS_TYPE" == *"mingw"* || "$OS_TYPE" == *"msys"* || "$OS_TYPE" == *"cygwin"* ]]; then
    SYSTEM="windows"
else
    echo "Système non reconnu : $OS_TYPE"
    exit 1
fi

echo "Détection du système : $SYSTEM"

# Lecture du fichier et export des variables
while IFS= read -r line || [[ -n "$line" ]]; do
    clean=$(echo "$line" | sed -E 's/^[[:space:]]+|[[:space:]]+$//g')

    # Ignorer vides ou commentaires
    [[ -z "$clean" || "$clean" =~ ^# ]] && continue

    # Extraire la variable et la valeur
    var=$(echo "$clean" | cut -d'=' -f1 | xargs)
    value=$(echo "$clean" | cut -d'=' -f2- | xargs | sed 's/^"//;s/"$//')

    if [[ "$SYSTEM" == "linux" ]]; then
        export "$var=$value"
        echo "$var défini (Linux)"
    elif [[ "$SYSTEM" == "windows" ]]; then
        powershell.exe -Command "[System.Environment]::SetEnvironmentVariable('$var','$value','User')"
        echo "$var défini (Windows)"
    fi
done < "$CONFIG_FILE"
