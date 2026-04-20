include <config.scad>
use <hinge.scad>
include <BOSL2/std.scad>
include <BOSL2/rounding.scad>


// ============================================
// MIDDLE TUBE SECTION
// ============================================

module middleSection() {
    difference() {
        cylinder(h = middleLength, d = middleWidth + 2*wallThickness);
        translate([0,0,-0.1]) 
            cylinder(h = middleLength + 1, d = middleWidth);
    }
}


// ============================================
// CONNECTOR (same as proximal)
// ============================================

module hinge_connector() {

    hull() {

        // Top profile
        translate([0, 0, 0])
        linear_extrude(height = connector_profile_height)
        square([
            jointThickness,
            2 * jointHeight + connector_flat_extra
        ], center = true);

        // Bottom profile
        translate([0, 0, -connector_drop])
        linear_extrude(height = connector_profile_height)
        square([
            jointThickness,
            connector_bottom_width
        ], center = true);
    }
}


// ============================================
// MIDDLE ASSEMBLY
// ============================================

module middleAssembly() {

    outerRadius = middleWidth/2 + wallThickness;

    union() {

        // ================= MAIN BODY =================
        middleSection();

        translate([
            -(outerRadius - wallThickness/2),
            0,
            middleLength + hinge_vertical_offset
        ]) {

            hinge_connector();

            hingeAssembly_male();
        }

        translate([
            (outerRadius - wallThickness/2),
            0,
            middleLength + hinge_vertical_offset
        ])
        mirror([1,0,0]) {
            hinge_connector();

            hingeAssembly_male();
        }
    }
}


// ============================================
// RENDER TEST
// ============================================
// hinge_connector();
middleAssembly();