import tkinter as tk
import configparser
from tkinter import filedialog


CONFIG_PATH = "UI/config.ini"
SECTION = "app"
FOLDER_PATH = "folder"

# Define colors
RED = "#ed3a59"
BLACK = "#001524"
BLUE = "#15616d"
LIGHT_BLUE = "#afc9cd"
WHITE = "#ffffff"

class FileSyncUI:
    pages = {}

    def __init__(self):
        self.root = tk.Tk()
        self.config = self.read_configuration_file(CONFIG_PATH)

        self.root.geometry("800x600")
        self.root.configure(bg=WHITE)
        self.root.resizable(False, False)
        self.root.title("FileSync")
        
        self.icon_image = tk.PhotoImage(file="UI/images/logo.png")
        self.logo_image = tk.PhotoImage(file="UI/images/small full logo white.png")
        self.root.tk.call('wm', 'iconphoto', self.root._w, self.icon_image)

        # Create header and footer
        self.create_footer()

        self.start()

    def create_header(self, frame, is_auth):
        header_frame = tk.Frame(frame, bg=BLACK)
        header_frame.pack(side="top", fill="x")

        self.logo_label = tk.Label(header_frame, image=self.logo_image, bg=BLACK)
        self.logo_label.pack(side="left")

        if is_auth:

            logout_label = tk.Label(header_frame, text="Logout", font=("Arial", 14), fg=WHITE, bg=BLACK)
            logout_label.bind("<Button-1>", lambda _: self.logout)
            logout_label.pack(side="right", padx=2, pady=10)

            welcome_label = tk.Label(header_frame, text=f"Welcome User   | ", font=("Arial", 14), fg=WHITE, bg=BLACK)
            welcome_label.pack(side="right", padx=2, pady=10)
            
        else:

            signup_label = tk.Label(header_frame, text="SignUp", font=("Arial", 14), fg=WHITE, bg=BLACK)
            signup_label.bind("<Button-1>", lambda _: self.navigate_to_page("signup"))
            signup_label.pack(side="right", padx=2, pady=10)

            login_label = tk.Label(header_frame, text="Login   | ", font=("Arial", 14), fg=WHITE, bg=BLACK)
            login_label.bind("<Button-1>", lambda _: self.navigate_to_page("login"))
            login_label.pack(side="right", padx=2, pady=10)

    def create_footer(self):
        # Create footer frame
        self.footer_frame = tk.Frame(self.root, bg=BLUE)
        self.footer_frame.pack(side="bottom", fill="x")

        # Create footer text
        self.footer_text = tk.Label(self.footer_frame, text="pelled.idan@gmail.com  |  Git repository  |  © 2023 FileSync All Rights Reserved.", fg=WHITE, bg=BLUE)
        self.footer_text.pack(side="top", padx=10, pady=5)

    def create_labe(self, frame, text, fg):
        label = tk.Label(frame, text=text, bg=WHITE, fg=fg)
        label.pack(side="top", padx=10, pady=5, anchor="w")
        return label
    
    def create_main_header(self, frame, text):
        header = tk.Label(frame, text=text, fg=RED, bg=WHITE, font=("Arial", 22, "bold"))
        header.pack(side="top", padx=10, pady=20, anchor="w")
        return header

    def create_text_box(self, frame, var ,**kw):
        text_box = tk.Entry(frame, background=LIGHT_BLUE, textvariable=var, **kw)
        text_box.pack(side="top", padx=10, pady=5, fill="x")
        return text_box

    def create_button(self, frame, text, bg, fg, action):
        button = tk.Button(
            frame,
            text=text,
            bg=bg,
            fg=fg,
            font=("Arial", 12, "bold"),
            highlightbackground=WHITE,
            command=action
        )
        button.pack(side="left", padx=10, pady=20)
        return button
    
    def create_login_page(self):
        # Create login frame
        self.login_frame = tk.Frame(self.root, bg=WHITE)
        self.create_header(self.login_frame, False)
        # Create login header
        self.login_header = self.create_main_header(self.login_frame, "Login")

        # Create username label and entry field
        self.username_label = self.create_labe(self.login_frame, "Username:", BLACK)
        self.login_form_username = tk.StringVar()
        self.username_entry = self.create_text_box(self.login_frame, self.login_form_username)

        # Create password label and entry field
        self.password_label = self.create_labe(self.login_frame, "Password:", BLACK)
        self.login_form_password = tk.StringVar()
        self.password_entry = self.create_text_box(self.login_frame, self.login_form_password, show="*")

        # Create error message label
        self.error_label = tk.Label(self.login_frame, text="", fg=RED, bg=WHITE)
        self.error_label.pack(side="top", padx=10, pady=5, anchor="w")

        # Create login button
        self.login_button = self.create_button(self.login_frame, "Login", RED, BLACK, self.login)

        # Pack login frame
        self.login_frame.pack(side="top", fill="both", expand=True)
        self.pages.update({"login": self.login_frame})

    def create_signup_page(self):
        # Create signup frame
        self.signup_frame = tk.Frame(self.root, bg=WHITE)
        self.create_header(self.signup_frame, False)
        # Create signup header
        self.signup_header = self.create_main_header(self.signup_frame, "Sign Up")
        
        # Create username label and entry field
        self.username_label = self.create_labe(self.signup_frame, "Username:", BLACK)
        self.signup_form_username = tk.StringVar()
        self.username_entry = self.create_text_box(self.signup_frame, self.signup_form_username)

        # Create password label and entry field
        self.password_label = self.create_labe(self.signup_frame, "Password:", BLACK)
        self.signup_form_password = tk.StringVar()
        self.password_entry = self.create_text_box(self.signup_frame, self.signup_form_password, show="*")

        # Create confirm password label and entry field
        self.confirm_password_label = self.create_labe(self.signup_frame, "Confirm password:", BLACK)
        self.signup_form_confirm_password = tk.StringVar()
        self.confirm_password_entry = self.create_text_box(self.signup_frame, self.signup_form_confirm_password, show="*")

        # Create signup button
        self.signup_button = self.create_button(self.signup_frame, "Sign Up", RED, BLACK, self.signup)

        # Pack signup frame
        self.signup_frame.pack(side="top", fill="both", expand=True)
        self.pages.update({"signup": self.signup_frame})

    def create_homepage(self):
        # Create homepage frame
        self.homepage_frame = tk.Frame(self.root, bg=WHITE)
        self.create_header(self.homepage_frame, True)

        # Create homepage header
        self.homepage_header = self.create_main_header(self.homepage_frame, "Syncronised Folder")

        # Create folder label and entry field
        self.folder_label = self.create_labe(self.homepage_frame, "Folder Path:", RED)
        self.folder_entry = self.create_labe(self.homepage_frame, self.get_folder_path(), BLACK)

        # Create change and save button
        self.change_folder_button = self.create_button(self.homepage_frame, "Change Folder", RED, BLACK, self.change_folder)
        self.save_folder_button = self.create_button(self.homepage_frame, "Save Folder", RED, BLACK, self.save_folder)
        
        # Pack homepage frame
        self.homepage_frame.pack(side="top", fill="both", expand=True)
        self.pages.update({"homepage": self.homepage_frame})

    def start(self):

        # Create login page
        self.create_login_page()

        # Create signup page
        self.create_signup_page()

        # Create homepage
        self.create_homepage()

        # Show login page
        self.navigate_to_page("login")

        self.root.mainloop()

    def navigate_to_page(self, page):
        self.hide_all()

        # Show selected page
        page = self.pages[page]
        page.update()
        page.pack(side="top", fill="both", expand=True)

    def hide_all(self):
        # Hide all pages
        for _, p in self.pages.items():
            p.pack_forget()
    
    def login(self):
        username = self.login_form_username.get()
        password = self.login_form_password.get()

        print(username, password)
        if username == "test" and password == "test":
            self.navigate_to_page("homepage")
        else:
            self.error_label.config(text="Incorrect username or password.")

    def signup(self):
        # Placeholder signup function
        print("Signing up...")

    def logout(self):
        self.navigate_to_page("login")
        print("Loggin out...")

    def change_folder(self):
        # Placeholder change folder function
        print("Changing folder...")
        self.folder_entry.config(text=filedialog.askdirectory())

    def save_folder(self):
        # Placeholder save folder function
        print("Saving folder...")
        self.save(self.folder_entry.cget('text'))

    def read_configuration_file(self, path: str) -> configparser.ConfigParser:
        config = configparser.ConfigParser()
        config.read(path)
        return config

    def get_folder_path(self) -> str:
        return self.config.get(SECTION, FOLDER_PATH)

    def update_folder_path(self, val: str):
        self.config.set(SECTION, FOLDER_PATH, val)

    def save_config_file(self, path: str):
        with open(path, 'w') as configfile:
            self.config.write(configfile)

    def save(self, path):
        self.update_folder_path(path)
        self.save_config_file(CONFIG_PATH)

if __name__ == "__main__":
    # Create and run the UI
    FileSyncUI()