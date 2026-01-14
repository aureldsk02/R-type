#!/bin/bash
echo -e "On commence par le formatage\nFormatage en cours...\n"
count=0
while IFS= read -r -d '' file; do
    count=$((count + 1))
    echo "Formatage de : $file"
    
    if ! clang-format -i "$file"; then
        echo "ERREUR sur : $file"
        exit 1
    fi
done < <(find engine/ rtype_client/ rtype_server/ utility_classes \( -name "*.cpp" -o -name "*.hpp" \) -print0)

if [ $count -eq 0 ]; then
    echo "Aucun fichier trouvé"
else
    echo "Formatage réussi sur $count fichier(s)"
fi

echo -e "It's time to compile\n"
if [ ! -d "build" ]; then
    mkdir build
fi
cd build
conan install .. --build=missing || exit 1
echo -e "Select a number to run a part of the project\n1.All\n2.Client\n3.Server\n"

while true; do
    read -p "Make your choice (1-3) : " choice
    if [[ $choice =~ ^[1-3]$ ]]; then
        break
    else
        echo "Enter a valid number between 1 and 3"
    fi
done

case $choice in 
    1) cmake .. -DBUILD_CLIENT=ON -DBUILD_SERVER=ON ;;
    2) cmake .. -DBUILD_CLIENT=ON -DBUILD_SERVER=OFF ;;
    3) cmake .. -DBUILD_CLIENT=OFF -DBUILD_SERVER=ON ;;
    *) echo "Choix invalide"; exit 1 ;;
esac

make
echo -e "Choose to run"


