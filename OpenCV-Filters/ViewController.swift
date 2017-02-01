//
//  ViewController.swift
//  OpenCV-Filters
//
//  Created by Justin Haupt on 1/17/17.
//  Copyright © 2017 Justin Haupt. All rights reserved.
//

import Cocoa
import CoreGraphics

class ViewController: NSViewController {

    @IBOutlet weak var ivInput: NSImageView!
    @IBOutlet weak var ivOutput: NSImageView!
    @IBOutlet weak var filterSelect: NSPopUpButton!
    @IBOutlet weak var slider1: NSSlider!
    @IBOutlet weak var slider2: NSSlider!
    @IBOutlet weak var slider3: NSSlider!
    @IBOutlet weak var slabel1: NSTextField!
    @IBOutlet weak var slabel2: NSTextField!
    @IBOutlet weak var slabel3: NSTextField!
    @IBOutlet weak var svalue1: NSTextField!
    @IBOutlet weak var svalue2: NSTextField!
    @IBOutlet weak var svalue3: NSTextField!
    
    var sourcePath: String?
    var outputFile: String?
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // initialize the filter menu
        let filters = ["Box filter", "Gaussian blur", "Median filter", "Bilateral filter",
                       "Canny edge detector", "Non-local mean filter", "Haar Wavelet denoise",
                       "Non-linear anisotropic diffusion smoothing (grayscale)"]
        
        filterSelect.removeAllItems()
        filterSelect.addItems(withTitles: filters)
        
        // initialize sliders
        self.slider3.isEnabled = false
    }

    
    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    
    @IBAction func importBtn(_ sender: Any) {
        
        let imageTypes = ["bmp", "dib", "jpg", "jpeg", "jpe", "jp2", "png",
                          "pbm", "pgm", "ppm", "sr", "ras", "tiff", "tif"]
        
        let op = NSOpenPanel()
        
        op.allowsMultipleSelection = false
        op.canChooseDirectories = false
        op.canChooseFiles = true
        
        op.runModal()
        
        let chosenFile = op.url
        
        let ext = chosenFile?.pathExtension.lowercased()
        
        if (chosenFile != nil && imageTypes.contains(ext!)) {
            self.sourcePath = String(chosenFile!.path)
            let picImport = NSImage(contentsOf: chosenFile!)
            self.ivInput.image = picImport
            self.ivOutput.image = nil
        }
    }
    
    
    @IBAction func exportBtn(_ sender: Any) {
        
        if(ivOutput.image != nil) {
            let op = NSOpenPanel()
            
            op.allowsMultipleSelection = false
            op.canChooseDirectories = true
            op.canCreateDirectories = true
            op.canChooseFiles = false
            
            op.runModal()
            
            let directory = op.url
            let url = directory!.path
            let path = String(url)! + "/" + self.outputFile!
            let toExport = self.ivOutput.image
            let cgImage = toExport?.cgImage(forProposedRect: nil, context: nil, hints: nil)
            let newRep = NSBitmapImageRep(cgImage: cgImage!)

            guard let data: NSData = newRep.representation(using: NSBitmapImageFileType.JPEG, properties: [NSImageCompressionFactor : 0.0]) as NSData?
                else { return }
            
            do {
                try data.write(toFile: path, options: NSData.WritingOptions.atomicWrite)
            }
            catch {
                Swift.print("Export image failed!")
            }
        }
    }
    
    @IBAction func filterSwitch(_ sender: Any) {
        let selection = Int(filterSelect.indexOfSelectedItem)
        switch selection{
        case 0:
            self.slider1.isEnabled = true
            self.slider2.isEnabled = true
            self.slider3.isEnabled = false
            self.slabel1.isHidden = false
            self.slabel2.isHidden = false
            self.slabel3.isHidden = true
            self.svalue1.isHidden = false
            self.svalue2.isHidden = false
            self.svalue3.isHidden = true
            self.slabel1.stringValue = "kernel size X"
            self.slabel2.stringValue = "kernel size Y"
        case 1:
            self.slider1.isEnabled = true
            self.slider2.isEnabled = false
            self.slider3.isEnabled = false
            self.slabel1.isHidden = false
            self.slabel2.isHidden = true
            self.slabel3.isHidden = true
            self.svalue1.isHidden = false
            self.svalue2.isHidden = true
            self.svalue3.isHidden = true
            self.slabel1.stringValue = "σ"
        case 2:
            self.slider1.isEnabled = true
            self.slider2.isEnabled = false
            self.slider3.isEnabled = false
            self.slabel1.isHidden = false
            self.slabel2.isHidden = true
            self.slabel3.isHidden = true
            self.svalue1.isHidden = false
            self.svalue2.isHidden = true
            self.svalue3.isHidden = true
            self.slabel1.stringValue = "kernel size"
        case 3:
            self.slider1.isEnabled = true
            self.slider2.isEnabled = true
            self.slider3.isEnabled = true
            self.slabel1.isHidden = false
            self.slabel2.isHidden = false
            self.slabel3.isHidden = false
            self.svalue1.isHidden = false
            self.svalue2.isHidden = false
            self.svalue3.isHidden = false
            self.slabel1.stringValue = "pixel diameter"
            self.slabel2.stringValue = "σ color"
            self.slabel3.stringValue = "σ space"
        case 4:
            self.slider1.isEnabled = true
            self.slider2.isEnabled = true
            self.slider3.isEnabled = false
            self.slabel1.isHidden = false
            self.slabel2.isHidden = false
            self.slabel3.isHidden = true
            self.svalue1.isHidden = false
            self.svalue2.isHidden = false
            self.svalue3.isHidden = true
            self.slabel1.stringValue = "threshold"
            self.slabel2.stringValue = "noise"
        case 5:
            self.slider1.isEnabled = true
            self.slider2.isEnabled = true
            self.slider3.isEnabled = false
            self.slabel1.isHidden = false
            self.slabel2.isHidden = false
            self.slabel3.isHidden = true
            self.svalue1.isHidden = false
            self.svalue2.isHidden = false
            self.svalue3.isHidden = true
            self.slabel1.stringValue = "luminance"
            self.slabel2.stringValue = "luminance color"
        case 6:
            self.slider1.isEnabled = true
            self.slider2.isEnabled = false
            self.slider3.isEnabled = false
            self.slabel1.isHidden = false
            self.slabel2.isHidden = true
            self.slabel3.isHidden = true
            self.svalue1.isHidden = false
            self.svalue2.isHidden = true
            self.svalue3.isHidden = true
            self.slabel1.stringValue = "threshold"
        case 7:
            self.slider1.isEnabled = true
            self.slider2.isEnabled = false
            self.slider3.isEnabled = false
            self.slabel1.isHidden = false
            self.slabel2.isHidden = true
            self.slabel3.isHidden = true
            self.svalue1.isHidden = false
            self.svalue2.isHidden = true
            self.svalue3.isHidden = true
            self.slabel1.stringValue = "gradient"
        default:
            self.slider1.isEnabled = false
            self.slider2.isEnabled = false
            self.slider3.isEnabled = false
            self.slabel1.isHidden = true
            self.slabel2.isHidden = true
            self.slabel3.isHidden = true
            self.svalue1.isHidden = true
            self.svalue2.isHidden = true
            self.svalue3.isHidden = true
        }
        
    }
    
    @IBAction func slider1Change(_ sender: Any) {
        let number = self.slider1.integerValue
        self.svalue1.stringValue = String(number)
    }
    
    @IBAction func slider2Change(_ sender: Any) {
        let number = self.slider2.integerValue
        self.svalue2.stringValue = String(number)
    }
    
    @IBAction func slider3Change(_ sender: Any) {
        let number = self.slider3.integerValue
        self.svalue3.stringValue = String(number)
    }

    
    @IBAction func goBtn(_ sender: Any) {
        
        if(ivInput.image != nil) {
            let selection = Int32(filterSelect.indexOfSelectedItem)
            let param1 = Int32(self.slider1.integerValue)
            let param2 = Int32(self.slider2.integerValue)
            let param3 = Int32(self.slider3.integerValue)

            self.outputFile = ObjFilters.filter(selection, source:self.sourcePath, option1:param1, option2:param2, option3:param3)
            self.ivOutput.image = NSImage(contentsOfFile: self.outputFile!)
        }
    }
}

