## 0.1.0




feature: add grid hit-testing and vertical layout support

* new grid hit-test API (MulleGridGetPositionAtPoint / MulleGridGetIndexAtPoint) to locate the grid cell at a coordinate
* new vertical layout mode (MulleLayoutInitVertical) that fills columns top-to-bottom
* new MulleLayoutGetUsedRect / MulleLayoutGetUsedSize helpers returning the bounding box of placed items
* layout geometry and corner/edge insets now use CGFloat instead of float
* edge and corner insets now validate against NaN and sanitize away -0.0 values
* fix CGRectAlignLeft assigning the y offset to the x axis


### 0.0.2

Various small improvements
