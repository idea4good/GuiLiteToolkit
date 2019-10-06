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
### For Linux Gcc - input unicode charaters directly
e.g: char* my_string = "abc你好"
### For Windows Visual studio
 - For ascii code: input directly.
 - For 2 bytes code or above, input charaters in code(e.g: char* my_string = "abc你好" => char* my_string = "abc **\xE4\xBD\xA0\xE5\xA5\xBD**")
 - How to get utf-8 code of chinese? Visit the websites below:
    - [Chinese website](http://www.mytju.com/classcode/tools/encode_utf8.asp)
    - [English website](https://www.browserling.com/tools/utf8-encode)

### Sample Code
[Link to HelloFont](https://github.com/idea4good/GuiLiteSamples/blob/master/HelloFont/UIcode/UIcode.cpp)

## How to build bitmap?
Step 1: Import 24 bits bitmap.

![import bitmap](doc/import_bitmap.jpg)

Step 2: Generate CPP files.

![generate](doc/generate_bitmap.jpg)

Step 3: Add CPP files to your APP project.

![addin](doc/addin_bitmap.jpg)

## Know issue
1. Letter lattice is all black.
- Because of DC error, the letter could not be display correctly, so the lattice would be wrong. The workaround is running GuiLiteToolkit in Visual Studio debug mode.
