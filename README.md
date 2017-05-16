A Real-time Gesture Recognition and Human-Computer Interaction System
======================================================================

This system is originally developed as my course project for computer vision.

This system mainly uses a modified LetNet-5 to recognize gestures, and call the operating system API to post keyboard and mouse events (which means that it can run in background) according to the recognition results.

A detailed introduction to this system can be found at https://arxiv.org/abs/1704.07296.

A demo video can be found at https://youtu.be/4n9F7iJJ2TY.


Dependencies
------------

This system depends on three libraries:

    Qt, v5.8
    OpenCV, v3.2.0
    Caffe, v1.0.0-rc5

During the test, the compilation is done by `qmake`. You need to modifies the path of the libraries in the `GestureRecognition.pro` file. Most of the libraries are required by Caffe.


Operating System Support
------------------------

This system calls OS API to implement the human-computer interaction. So far, it only supports MAC OS. You need to modify `CommandInputter.h` and `CommandInputterInterface.h` if you want to make it work on other systems. You can just search `TODO` in these two files.

The support to Windows and Linux (using X11) will be completed recently.

Documentation
-------------

Run `doxygen` at the folder `doc`.

Model
-----

A trained models using the image sizes of 64x64 is provided in the `data` folder. I trained three models respectively using samples with the size of 128x128, 64x64 and 32x32. For each size, I collected 19,852 samples of 16 kinds of gestures by myself. They can be found at the `samples` folder. All of the samples are in `PGM` format. The training log of the three models can be found at the `log` folder. The network structure is defined by `lenet.prototxt` at the folder `data`. We suggest to use the model with size 64x64. A model with size 64x64 is provided in `data` folder. If you want to use other size but not 64x64, you need to modify `SAMPLE_SIZE_WIDTH` and `SAMPLE_SIZE_HEIGHT` in `global.h` or specify the values for them when compiling, and you need to train the model yourself.

All samples you can use freely but please note where you get them, i.e. the address of this page or the reference of my report at https://arxiv.org/abs/1704.07296.

Configuration File
------------------

We use an independent keymap configuration file to tell the system what it needs to do when facing a gesture. Two sample configuration files can be found in the folder `data`. Each keymap configuration file has three fields.

The `labels` filed defines the label for each gesture. Their order should be consistent with the label index using in the model trained by Caffe.

The `mouse-actions` filed are the indices of the gestures used to control mouse. They are separated by `!x0x?` and used to identify the gestures for mouse 'move', 'left click', 'right click', 'drag' and 'double left click' respectively.

The `key-shortcuts` field defines what key or what combination of keys the system should simulate to press when it faces a gestures. You can use '+' to link multiple keys and obtain a combination of keyboard events. The names of keys are defined by the enum `MOUSE_KEYBOARD_ACTION` in the `CommandInputter.h` file.

If you want the system to execute more complex actions, you may need to modify the `CommandInputter.cpp` file.

The `key-map` file is designed to command the computer to post keyboard and mouse events. However, you can easily reconstruct the system by modifying the `CommandInputter` class and make it execute more complex actions based on gesture recognition.

How This System Work?
---------------------

- Successfully compile it on a MAC OS platform firstly.
- Launch the system and click on the gesture control button.
- Load the keymap configuration file and model file
- Click on the control button to start controlling (You may want to open the monitor window and setting window to adjust the background filter beforehand).


TODO
----

- Support to Windows and Linux (X11) platforms.
- GUI for the function of `key-map` file modification.
- Extend into the Human-Robotics Interaction scenarios.
