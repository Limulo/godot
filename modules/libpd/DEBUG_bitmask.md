## How does Import Plugin works? 

### A debug test on EditorBitMaskImportPlugin

As soon as the editor starts:

	DEBUG => EditorBitMaskImportPlugin constructor 1
	DEBUG => EditorBitMaskImportDialog constructor
	DEBUG => EditorBitMaskImportPlugin::get_name
	DEBUG => EditorBitMaskImportPlugin::get_name
	DEBUG => EditorBitMaskImportPlugin::get_visidebug_bitmaskble_name

When we select the `BitMask` option in the `Import` menu:

	DEBUG => EditorBitMaskImportPlugin::import_dialog
	DEBUG => EditorBitMaskImportDialog->popup_import

When we have to select the file to import:

	DEBUG => EditorBitMaskImportDialog->_browse 

We can then browse in our filesystem inside a dedicated window. 
As soon as we select the file we want to import:

	DEBUG => EditorBitMaskImportDialog->_choose_files

Then we're asked to choose a target location for the imported resource

	DEBUG => EditorBitMaskImportDialog->_browse_target

again we're offered a browser view.
Once selected the destination folder:

	DEBUG => EditorBitMaskImportDialog->_choose_save_dir

Eventually, when clicking on the `Import` button:

	DEBUG => EditorBitMaskImportDialog->_import
	DEBUG => EditorBitMaskImportPlugin::import
	DEBUG => EditorBitMaskImportPlugin::get_name
	SAVING: res://perrybara.pbm
	SAVE PATH: ../../../Immagini/screenshots/perrybara.png
	SAVE MD5: df4b8dc894988bcc009bf941a8bdec1c

The last three lines are output from functions in the `core/io/resource_format_binary.cpp` file.


