#!/usr/bin/env python3

import os

def create_level():
    output_dir = "assets/levels"
    os.makedirs(output_dir, exist_ok=True)
    
    filename = input("File name: ").strip()
    
    if not filename:
        filename = "level"
    if not filename.endswith(".txt"):
        filename += ".txt"

    filepath = os.path.join(output_dir, filename)

    entities = []
    
    entity_types = [
        "red_trooper",
        "walker",
        "floor",
        "ceiling",
        "boss",
        "plane",
        "force",
        "green_trooper",
        "tourelles",
        "big_shooter",
        "small_shooter",
        "final_boss",
        "space_enemy",
    ]
    
    while True:
        print("Choose an entity:")
     
        for i, entity_type in enumerate(entity_types, 1):
            print(f"  {i}- {entity_type}")
        
        choice = input("\nEntity number (ou 'q' to close): ").strip()
        
        if choice.lower() == 'q':
            break
        
        try:
            choice_num = int(choice)
            if 1 <= choice_num <= len(entity_types):
                name = entity_types[choice_num - 1]
            else:
                print("Invalid Number, try again.")
                continue
            
        except ValueError:
            print("Invalid Input, try again.")
            continue
        
        try:
            spawn_time = float(input("Spawn Time: "))
            y = float(input("Position Y: "))
            
            entities.append({
                'name': name,
                'spawn_time': spawn_time,
                'y': y
            })
            
            print(f" Input: {name} à t = {spawn_time}s, y = {y}")
            
            continuer = input("\nAdd another entity ? (put 'ok' to continue, press enter to close): ").strip()
            if continuer.lower() != 'ok':
                break
            
        except ValueError:
            print("Error: Please enter valid numbers for spawn_time and postion y")
    
    if entities:
        
        content = "entities = (\n"
        for i, entity in enumerate(entities):
            content += f'    {{name = "{entity["name"]}"; spawn_time = {entity["spawn_time"]}; y = {entity["y"]};}}' 
            if i < len(entities) - 1:
                content += ","
            content += "\n"
        content += ");"
        
        with open(filepath, 'w') as f:
            f.write(content)
        
        print(f"\nFile '{filename}' create with success!")
        print(f" {len(entities)} entitie(s) add")
        print(f"\nContent:\n{content}")
    else:
        print("\nNone entity add, file not create")

if __name__ == "__main__":
    create_level()
