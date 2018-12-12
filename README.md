# GuiLite Toolkit - Build Font/Bitmap resource
GuiLiteToolkit is 200 lines code for building font lattice & bitmap of [GuiLite](https://github.com/idea4good/GuiLite).

## How to build font lattice?
Step 1: Input the charaters you need.

![input](doc/input.jpg)

Step 2: Choose font type you want.

![choose.font](doc/choose.font.jpg)

Step 3: Generate CPP files.

![generate](doc/generate_font.jpg)

Step 4: Add CPP files to your APP project.

![addin](doc/addin_font.jpg)

## How to input charaters in your APP?
It works with unicode(UTF-8), will help your APP support all languages and symbols all over the world.
### For Linux - input unicode charaters directly
e.g: char* my_string = "abc你好"
### For Windows Visual studio
 - For ascii code: input directly.
 - For 2 bytes code or above, input charaters in code(You can get the code for Chinese word with this [website](https://www.browserling.com/tools/utf8-encode)): 
 
 e.g: char* my_string = "abc你好" => char* my_string = "abc\xE4\xBD\xA0\xE5\xA5\xBD"
 
## How to build bitmap?
Step 1: Import 24 bits bitmap.

![import bitmap](doc/import_bitmap.jpg)

Step 2: Generate CPP files.

![generate](doc/generate_bitmap.jpg)

Step 3: Add CPP files to your APP project.

![addin](doc/addin_bitmap.jpg)

 ## Sample code
 [GuiLite Samples](https://github.com/idea4good/GuiLiteSamples/blob/master/HostMonitor/SampleCode/source/ui_tree/Dialog/patient_setup/patient_setup_dlg_xml.cpp)
