### 0.0.2


feature: export row content rects; make grid layout robust to infinite/invalid canvases

* Export `_MulleLayoutGetRowContentRect` and `_MulleLayoutGetRemainingRowContentRect` (marked `MULLE__LAYOUT_GLOBAL)` so callers can obtain row content rectangles.
* MulleGridConstruct now sets p->canvas = CGRectNull and returns 0 on failure; MulleGridVerify returns 0 for invalid layouts, allowing callers to detect invalid/empty layouts reliably.
* Grid computations now handle infinite canvas dimensions and zero rows/columns gracefully, preventing invalid sizes and avoiding crashes.










* add MulleGrid to project

* add new demo

* added `MulleEdgeInsetsMakeLR` convenience constructor
* enable struct MulleLayout to work from right to left also
* changed the ways margins work in MulleLayout, hopefully for the better
* typedefs are now NSUInteger instead of enums
* new functions `CGSizeAspectFitToSize` and `CGSizeAspectFillToSize`
* new functions `CGRectMirrorInRectH` and `CGRectMirrorInRectV`
* added some partitioning conveniences






* have union altenative to base type for easier axis handling



* moved quadratic to MulleCA
* moved affine to MulleCG
* death of the Mulle prefix for enums, structs and functions

* split MulleLayout off from MulleUI


* nach dem Fundamentalumbau mit CALayer im windowspace und der



* new demo render-png to render out a PNG file








* UITextField added

* fix UIView conversion toView
* much improved MullePopUpButton
* layer drawing methods now return a YES if transform or scissor have been affected

* MulleJS is now its own project in MulleWeb
* MulleMenu and MullePopUpButton added (rename to MulleUIMenu ...) ?


* color creation with strings
* javascript interface for nanovg
* new method setSubviews: on UIView
* new UIStackView layout MulleStackViewDistributionFillRowColumn









* CALayer now uses introspection to generate animations for observable properties

































* experimental yoga layouting

* add TODO list to README.md

* improved SVG rendering

* redid CALayer background drawing and border



* UIImage added. UIButton can now change its background image

* UIControl as protocolclass added

* modernized mulle-sde
* UIEvents are distributed to subviews properly
