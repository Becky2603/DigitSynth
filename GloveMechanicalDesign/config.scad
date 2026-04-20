// ============================================
// GLOVE MECHANICAL DESIGN PARAMETERS
// ============================================


// ==========================
// FINGER DIMENSIONS (mm)
// ==========================
proximalWidth  = 20;
middleWidth    = 16;

proximalLength = 25;
middleLength   = 20;


// ==========================
// MATERIAL
// ==========================
wallThickness  = 2;


// ==========================
// HINGE CORE GEOMETRY
// ==========================
jointHeight    = 10;                 // hinge radius
jointThickness = wallThickness;      // hinge thickness in X direction


// ==========================
// PIN
// ==========================
pin_d                = 3;
pinRadius            = pin_d / 2;
pin_vertical_offset  = 2;            // mm downward shift


// ==========================
// HINGE POSITIONING
// ==========================
hinge_vertical_offset = 5;           // mm upward shift of hinge connector


// ==========================
// MALE / FEMALE FIT
// ==========================
male_width      = jointThickness * 0.4;
hinge_clearance = 0.3;               // printing tolerance


// ==========================
// CONNECTOR GEOMETRY
// ==========================
connector_bottom_width   = 10;
connector_drop           = 20;
connector_profile_height = 1;
connector_flat_extra     = 1;        // replaces +1 in square height