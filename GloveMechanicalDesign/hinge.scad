include <config.scad>







module hinge_block() {

    difference() {

        // Full cylinder (lying on its side)
        rotate([0,90,0])
            cylinder(
                h = jointThickness,
                r = jointHeight,
                center = true
            );

        // Remove bottom half to create semi-cylinder
        translate([
            -jointThickness,
            -jointHeight,
            -jointHeight
        ])
            cube([
                2*jointThickness,
                2*jointHeight,
                jointHeight
            ]);
    }
}


module hinge_block_female(
    thickness = jointThickness,
    radius = jointHeight,
    male_width = jointThickness * 0.4,
    clearance = 0.3
) {

    difference() {

        // Original semi-cylinder
        difference() {

            rotate([0,90,0])
                cylinder(h = thickness, r = radius, center = true);

            translate([
                -thickness,
                -radius,
                -radius
            ])
                cube([
                    2*thickness,
                    2*radius,
                    radius
                ]);
        }

        // Remove slot for male part
        cube([
            male_width + clearance,
            2*radius,
            2*radius
        ], center=true);
    }
}

module hinge_block_male(
    thickness = jointThickness,
    radius = jointHeight,
    male_width = jointThickness * 0.4
) {
    translate([ male_width/2, 0, 0 ])   // keep in +X
    intersection() {

        // Narrow cylinder
        rotate([0,90,0])
            cylinder(h = male_width, r = radius, center = true);

        // Remove BOTTOM half (keep TOP)
        translate([ -male_width, -radius, 0 ])
            cube([ 2*male_width, 2*radius, radius ]);
    }
}
module hinge_base() {

    difference() {

        // rectangular base
        cube([
            jointThickness,
            2 * jointHeight,
            wallThickness
        ], center=false);

        // subtract matching cylinder curvature
        translate([
            jointThickness/2,
            jointHeight,
            -proximalLength
        ])
            cylinder(
                h = proximalLength + 5,
                d = proximalWidth + 2*wallThickness
            );
    }
}
// =====================
// MODULE: Hinge Pin Hole
// =====================
module hinge_pin_hole() {

    translate([0, 0, (jointHeight / 2) - pin_vertical_offset])
        rotate([0,90,0])
            cylinder(
                h = jointThickness + 2,
                d = pin_d,
                center = true,
                $fn = 60
            );
}



module hingeAssembly()
{
    union() {

        // ----------------------
        // Base (positioned under hinge)
        // ----------------------
        translate([
            -jointThickness/2,
            -jointHeight,
            -wallThickness
        ])
            hinge_base();


        // ----------------------
        // Hinge block with pin hole removed
        // ----------------------
        difference() {

            hinge_block();

            hinge_pin_hole();
        }
    }
}

module hingeAssembly_male()
{
    union() {

        // ----------------------
        // Base
        // ----------------------
        translate([
            -jointThickness/2,
            -jointHeight,
            -wallThickness
        ])
            hinge_base();

        // ----------------------
        // Male hinge block
        // ----------------------
        difference() {

            hinge_block_male();

            // Add the pin hole (same axis as female)
            hinge_pin_hole();
        }
    }
}
module hingeAssembly_female()
{
    union() {

        // ----------------------
        // Base
        // ----------------------
        translate([
            -jointThickness/2,
            -jointHeight,
            -wallThickness
        ])
            hinge_base();

        // ----------------------
        // Female hinge block
        // ----------------------
        difference() {

            hinge_block_female();

            hinge_pin_hole();
        }
    }
}
hingeAssembly_female();
              // printing tolerance
translate([-(hinge_clearance),0,0]) 
hingeAssembly_male();