**FONTMAKER TOOL by ZipiRo**  
*Using the [PiXELGraph](https://github.com/ZipiRo/PiXELGraph.git) library*  
*For creating fonts for PiXELGraph library*  

---

### **INFO**  
- **Canvas size:** 11x15 (per character)  
- **Resolution:** 10 (0.1) 
- **BottomLine:** Is a border from 1.1 -> 1.5 for some characters, just to know  

---

### **POINT MODE**  
- **Cursor color:** Red  
- **Points color:** Blue  

#### **Controls:**  
- **Arrow Keys:** Move the cursor  
- **Enter Key:** Place a point at the cursor position  
- **(-):** Remove the last placed point  
- **P:** Show the points you’ve placed  
- **C:** Toggle crosshair (vertical and horizontal lines that follow the cursor)  
- **Q:** Switch to indices mode (connect points)  
- **Shift + S:** Save the glyph (doesn't work if CAPSLOCK ON)
  - *You will be prompted to enter the character for the glyph.*  
  - *The file will be saved as `glyphdata.txt` in the program folder.*  

---

### **INDICES MODE**  
- **Cursor color:** Purple  
- **Index One color:** Intense Green  
- **Index Two color:** Cyan  

#### **Controls:**  
- **Arrow Keys:** Move the cursor  
- **Enter Key:**  
  - First press: Place Index One  
  - Second press: Place Index Two  
- **Q:** Switch to point mode (place points)  
- **Shift + R:** Restart indices mode (doesn't work if CAPSLOCK ON)
- **Shift + S:** Save the glyph (doesn't work if CAPSLOCK ON)
  - After saving, an overview of the connected points (with black lines) is shown.  
  - You’ll return to point mode after saving, but editing is locked unless you:  
    - Switch to indices mode (`Q`).  
    - Restart indices (`Shift + R`).  

---

### **SAVE FILE**  
- The saved file is a **JSON snippet**.  
- You can copy and paste the JSON directly.  
- **Note:** There is an `advance` keyword in the JSON for advanced settings.  

#### **Example JSON Structure:**  
```json
"character": {
    "vertices": [],
    "indices": [],
    "advance": 0
},
```

--- 
