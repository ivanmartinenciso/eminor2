package main

import (
	"image"
	"image/color"
	"time"
)
import "github.com/andlabs/ui"
import "github.com/JamesDunne/eminor2/v2/controller/go-ui/simulation"
import "code.google.com/p/draw2d/draw2d"

const mmToIn = 0.0393701

// display scale factor (pixels per inch)
// NOTE(jsd): 55.4 dpi is to-scale on my 40" Samsung HDTV 1080p
const defaultDpi = 55.4
const dpi = 93.5

// Total width, height in inches:
const inWidth = 20.078
const inHeight = 6.305

// Position and spacing of footswitches (from centers):
const hLeft = 1.0
const hSpacing = 2.5714285714285714285714285714286

// From bottom going up:
const vStart = 5.6
const vSpacing = 2.15

const vLEDOffset = -0.65

const inLEDOuterDiam = (12 /*mm*/ * mmToIn)
const inFswOuterDiam = (12.2 /*mm*/ * mmToIn)
const inFswInnerDiam = (10 /*mm*/ * mmToIn)

// button labels:
var labels = [][]string{
	[]string{"CH1", "CH1S", "CH2", "CH2S", "CH3", "CH3S", "TAP/STORE", "NEXT"},
	[]string{"COMP", "FILTER", "PITCH", "CHORUS", "DELAY", "REVERB", "MUTE", "PREV"},
}

var keylabels = [][]string{
	[]string{"A", "S", "D", "F", "G", "H", "J", "K"},
	[]string{"Q", "W", "E", "R", "T", "Y", "U", "I"},
}

type canvasArea struct {
	img *image.RGBA
}

// Paint is called when the Area needs to be redrawn.
// The part of the Area that needs to be redrawn is stored in cliprect.
// Before Paint() is called, this region is cleared with a system-defined background color.
// You MUST handle this event, and you MUST return a valid image, otherwise deadlocks and panicking will occur.
// The image returned must have the same size as rect (but does not have to have the same origin points).
func (area *canvasArea) Paint(cliprect image.Rectangle) *image.RGBA {
	//area.img.SetRGBA(0, 0, color.RGBA{0, 0, 0, 255})
	dc := draw2d.NewGraphicContext(area.img)
	dc.Save()
	dc.SetFillColor(color.Black)
	draw2d.Ellipse(dc, 16, 16, 8, 8)
	dc.FillStroke()
	dc.Restore()

	return area.img.SubImage(cliprect).(*image.RGBA)
}

// Mouse is called when the Area receives a mouse event.
// After handling the mouse event, package ui will decide whether to perform platform-dependent event chain continuation based on that platform's designated action (so it is not possible to override global mouse events this way).
func (area *canvasArea) Mouse(e ui.MouseEvent) {
	return
}

// Key is called when the Area receives a keyboard event.
// Return true to indicate that you handled the event; return false to indicate that you did not and let the system handle the event.
func (area *canvasArea) Key(e ui.KeyEvent) (handled bool) {
	return false
}

func main() {
	simulation.Init()

	timer_10ms := time.Tick(10 * time.Millisecond)

	go ui.Do(func() {
		widthFloat := float64(inWidth * dpi)
		heightFloat := float64(inHeight * dpi)
		width := int(widthFloat)
		height := int(heightFloat)

		// Give ourselves a drawable canvas:
		canvas := &canvasArea{
			img: image.NewRGBA(image.Rect(0, 0, width, height)),
		}
		area := ui.NewArea(width, height, canvas)
		w := ui.NewWindow("e-minor v2", width+8*2, height+28*2, area)
		w.SetMargined(false)
		w.OnClosing(func() bool {
			ui.Stop()
			return true
		})

		w.Show()

		// This *should* be in the main UI message loop, but the 'ui' package does not expose the message loop.
		go func() {
			for {
				select {
				case <-timer_10ms:
					simulation.Timer_10msec()
				default:
					simulation.Handle()
				}
			}
		}()
	})

	err := ui.Go()
	if err != nil {
		panic(err)
	}
}
