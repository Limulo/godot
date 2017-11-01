## How does Import Plugin works? 

### A debug test on BitMap resource file

As soon as Godot is launched:

	DEBUG => BitMap->_bind_methods

(we're still in the project list window)

Once the project we want to open is selected, and the `edit` command is executed:

	DEBUG => EditorBitMaskImportPlugin constructor 1
	DEBUG => EditorBitMaskImportDialog constructor
	DEBUG => EditorBitMaskImportDialog->_bind_methods
	DEBUG => EditorBitMaskImportPlugin::get_name
	DEBUG => EditorBitMaskImportPlugin::get_name
	DEBUG => EditorBitMaskImportPlugin::get_visible_name



A popup window appears

	DEBUG => EditorBitMaskImportDialog->popup_import

We can brwose the FS and choose the file to import

	DEBUG => EditorBitMaskImportDialog->_browse

Once the file has been chose:

	DEBUG => EditorBitMaskImportDialog->_choose_files

We have to select a target path

	DEBUG => EditorBitMaskImportDialog->_browse_target

and when we choose it

	DEBUG => EditorBitMaskImportDialog->_choose_save_dir

Then

	DEBUG => EditorBitMaskImportDialog->_import
	DEBUG => EditorBitMaskImportPlugin::import

	DEBUG => BitMap constructor
	DEBUG => BitMap::create_from_image_alpha
	DEBUG => BitMap::create

	DEBUG => EditorBitMaskImportPlugin::get_name
	
	DEBUG => BitMap::_get_data
	DEBUG => BitMap::_get_data

	SAVING: res://perrybara.pbm
	SAVE PATH: ../../../Immagini/screenshots/perrybara.png
	SAVE MD5: df4b8dc894988bcc009bf941a8bdec1c




