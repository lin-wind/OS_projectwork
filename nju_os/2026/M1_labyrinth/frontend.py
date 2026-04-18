import subprocess
import sys
import os

# Platform specific imports for catching single keystroke without enter
if os.name == 'nt':
    try:
        import msvcrt
    except ImportError:
        pass
else:
    import termios
    import tty

# Key mappings to (player_id, direction_string)
KEY_MAP = {
    # Player 0: W A S D
    'w': (0, 'up'),
    'a': (0, 'left'),
    's': (0, 'down'),
    'd': (0, 'right'),
    
    # Player 1: H J K L
    'k': (1, 'up'),
    'h': (1, 'left'),
    'j': (1, 'down'),
    'l': (1, 'right'),
}

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

def get_char():
    """Cross-platform function to read a single character from terminal"""
    if os.name == 'nt':
        char = msvcrt.getwch()
        # Handle special keys in Windows (arrows, etc. return two bytes)
        if char in ('\x00', '\xe0'):
            msvcrt.getwch() # consume second byte
        return char.lower()
    else:
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch.lower()

def main():

    if len(sys.argv) > 1:
        map_file_path = sys.argv[1]
    else:
        map_file_path = "map.txt"

    if not os.path.exists(map_file_path):
        print(f"\r\n[错误]: 找不到地图文件 '{map_file_path}'")
        sys.exit(1)

    cli_path = r".\bin\labyrinth.exe" if os.name == 'nt' else "./bin/labyrinth"

    if not os.path.exists(map_file_path):
        print(f"Error: Initial map file '{map_file_path}' not found.")
        sys.exit(1)
        
    if not os.path.exists(cli_path):
        print(f"Error: CLI backend '{cli_path}' not found. Please compile it first using 'make'.")
        sys.exit(1)

    # Load initial map state into memory string
    with open(map_file_path, "r", encoding="utf-8") as f:
        current_map = f.read()

    print("Welcome to Local Co-op Labyrinth!")
    print("Player 0: W/A/S/D  |  Player 1: H/J/K/L")
    print("Press 'Q' or 'ESC' to quit.\n")
    print("Press any key to start...")
    get_char()

    game_msg = ""

    while True:
        clear_screen()
        print("======== Labyrinth PvP Mode ========")
        print("P0: WASD | P1: HJKL | Q/ESC: Quit")
        print("------------------------------------")
        print(current_map, end='')  # map already includes newlines

        if game_msg:
            print(f"\n[提示]: {game_msg}")
            game_msg = ""
        else:
            print("\n")

        # Block and read single keystroke
        char = get_char()
        
        # Handle Exit
        if char == 'q' or char == '\x1b': # ESC
            print("\r\nExiting game...") # \r is needed because of tty raw mode
            break

        if char not in KEY_MAP:
            continue

        player_id, direction = KEY_MAP[char]

        # Call C backend process passing current map via stdin
        cmd = [
            cli_path,
            "--player", str(player_id),
            "--move", direction
        ]
        
        try:
            result = subprocess.run(
                cmd,
                input=current_map,
                text=True,
                capture_output=True
            )
            
            # If valid move, update current_map. 
            if result.returncode == 0:
                current_map = result.stdout
            else:
                game_msg = result.stderr.strip()

            
        except Exception as e:
            game_msg = f"Error invoking backend: {e}"

if __name__ == "__main__":
    main()
