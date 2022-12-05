void hierarchical_cyl(double depth, double diffPct, double reflPct, double tranPct, double refl_sig,
                      double r_index, double direction);

void hierarchical_circ(double ref[4][4], double depth, double diffPct, double reflPct, double tranPct, double refl_sig,
                      double r_index, double direction);

void buildScene(void) {
    // Sets up all objets in the scene. This involves creating each object,
    // defining the transformations needed to shape and position it as
    // desired, specifying the reflectance properties (albedos and colours)
    // and setting up textures where needed.
    //
    // NOTE: Light sources are now EXCLUSIVELY area light sources. They are
    //       defined by regular objects whose 'isLightSource' flag is set
    //       to 1. Therefore, you can create light sources with any shape
    //       and colour using the same object primitives and transforms
    //       you're using to set up the scene.
    //
    // To create hierarchical objects:
    //    You must keep track of transformations carried out by parent objects
    //    as you move through the hierarchy. Declare and manipulate your own
    //    transformation matrices (use the provided functions in utils.c to
    //    compound transformations on these matrices). When declaring a new
    //    object within the hierarchy
    //    - Initialize the object
    //    - Apply any object-level transforms to shape/rotate/resize/move
    //      the object using regular object transformation functions
    //    - Apply the transformations passed on from the parent object
    //      by pre-multiplying the matrix containing the parent's transforms
    //      with the object's own transformation matrix.
    //    - Compute and store the object's inverse transform as usual.
    //
    // NOTE: After setting up the transformations for each object, don't
    //       forget to set up the inverse transform matrix!

    struct object3D *o;
    struct point3D p;

    // Cornell box
    o = newPlane(1.0, 0.0, 0.0, .75, .75, .75, .02, 1.4);    // Left
    Scale(o, 20, 20, 20);
    RotateY(o, PI / 2);
    Translate(o, -14, 0, 10);
    loadTexture(o, "./texture/wall.ppm", 1, &texture_list);
    loadTexture(o, "./texture/nwall.ppm", 2, &texture_list);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);

    o = newPlane(1.0, 0.0, 0.0, .75, .75, .75, .02, 1.4);    // Right
    Scale(o, 20, 20, 20);
    RotateY(o, PI / 2);
    Translate(o, 14, 0, 10);
    loadTexture(o, "./texture/wall.ppm", 1, &texture_list);
    loadTexture(o, "./texture/nwall.ppm", 2, &texture_list);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);


    o = newPlane(1.0, 0.0, 0.0, .75, .75, .75, .05, 1.4);        // Back
    Scale(o, 15, 12, 12);
    RotateZ(o, PI);
    Translate(o, 0, 0, 20);
    loadTexture(o, "./texture/back.ppm", 1, &texture_list);
    loadTexture(o, "./texture/nback.ppm", 2, &texture_list);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);


    o = newSphere(1.0, 0.0, 0.0, .75, .75, .75, .05, 1.4);        // front
    Scale(o, 500, 500, 500);
    Translate(o, 0, 0, -515);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);

    o = newPlane(1.0, 0.0, 0.0, .75, .75, .75, .02, 1.4);    // Bottom
    Scale(o, 20, 20, 20);
    RotateX(o, PI / 2);
    Translate(o, 0, -10, 10);
    loadTexture(o, "./texture/ground.ppm", 1, &texture_list);
    loadTexture(o, "./texture/nground.ppm", 2, &texture_list);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);

    o = newSphere(1.0, 0.0, 0.0, .75, .75, .75, .05, 1.4);        // Top
    Scale(o, 500, 500, 500);
    Translate(o, 0, 510, 5);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);

    // tree
    o = newCyl(1.0, 0.0, 0.0, 1, 1, 1, .01, 1.54);
    Scale(o, 1, 1, 20);
    RotateX(o, PI / 2);
    Translate(o, 0, -2, 5.5);
    loadTexture(o, "./texture/body.ppm", 1, &texture_list);
    loadTexture(o, "./texture/nbody.ppm", 2, &texture_list);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);

    hierarchical_cyl(8, 1, 0, 0, 0.01, 1.54, 1);
    hierarchical_cyl(8, 1, 0, 0, 0.01, 1.54, -1);


    // Planar light source at top
    o = newPlane(1.00, 0.00, 0.0, 1.0, 1.0, 1.0, 0.0, 1.54);
    Scale(o, 2, 8, 3);
    RotateX(o, PI / 2);
    Translate(o, 0, 10, 9);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    o->isLightSource = 1;
    insertObject(o, &object_list);
}

void hierarchical_cyl(double depth, double diffPct, double reflPct, double tranPct, double refl_sig,
                      double r_index, double direction) {
    if (depth > 0) {
        double coef = (8 - depth);
        struct object3D *o = newCyl(diffPct, reflPct, tranPct, 1, 1, 1, refl_sig, r_index);
        double ref[4][4] = {{1.0, 0.0, 0.0, 0.0},
                            {0.0, 1.0, 0.0, 0.0},
                            {0.0, 0.0, 1.0, 0.0},
                            {0.0, 0.0, 0.0, 1.0}};
        Scale(o, .4, .4, 8 - 0.6 * coef);
        RotateY(o, PI / 2);
        Translate(o, direction * (4 - 0.3 * coef), -6 + 1.5 * coef, 0);
        TranslateMat(ref, direction * (4 - 0.3 * coef), -6 + 1.5 * coef, 0);
        if (direction == 1) {
            RotateY(o, coef * PI / 3);
            RotateYMat(ref, coef * PI / 3);
        } else {
            RotateY(o, coef * PI / 3.5);
            RotateYMat(ref, coef * PI / 3.5);
        }
        Translate(o, 0, 0, 5.5);
        loadTexture(o, "./texture/leg.ppm", 1, &texture_list);
        loadTexture(o, "./texture/nleg.ppm", 2, &texture_list);
        invert(&o->T[0][0], &o->Tinv[0][0]);
        insertObject(o, &object_list);

        struct object3D *hat = newSphere(0, 1, 0, drand48(), drand48(), drand48(), refl_sig, r_index);
        Scale(hat, .6, .6, .6);
        Translate(hat, 2 * direction * (4-0.3*coef), -6 + 1.5 * coef, 0);
        if (direction == 1) {
            RotateY(hat, coef * PI / 3);
        } else {
            RotateY(hat, coef * PI / 3.5);
        }
        Translate(hat, 0, 0, 5.5);
        invert(&hat->T[0][0], &hat->Tinv[0][0]);
        insertObject(hat, &object_list);

        hierarchical_circ(ref, 2 * depth - 1, diffPct, reflPct, tranPct, refl_sig, r_index, direction);
        hierarchical_cyl(depth - 1, diffPct, reflPct, tranPct, refl_sig, r_index, direction);
    }
}

void hierarchical_circ(double ref[4][4], double depth, double diffPct, double reflPct, double tranPct, double refl_sig,
                       double r_index, double direction) {
    if (depth > 0) {
        double coef = (8 - depth);
        struct object3D *o = newSphere(0, 1, 0, drand48(), drand48(), drand48(), refl_sig, r_index);
        Scale(o, .25, .25, .25);
        Translate(o, direction * (2 - 0.2 * coef), .7, 0);
        RotateX(o, coef * PI / 3 );
        matMult(ref, o->T);
        Translate(o, 0, 0, 5.5);
        invert(&o->T[0][0], &o->Tinv[0][0]);
        insertObject(o, &object_list);
        hierarchical_circ(ref, depth - 1, diffPct, reflPct, tranPct, refl_sig, r_index, direction);
    }
}