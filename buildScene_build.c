void hierarchical(double M1[4][4], double depth, double diffPct, double reflPct, double tranPct, double refl_sig,
                  double r_index, double orientaion, double loop);

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

//  o=newPlane(1.0,0.0,0.0,.75,.25,.25,.05,1.4);	// Left
//  Scale(o,10,10,10);
//  RotateZ(o,PI/2);
//  Translate(o,-5,0,0);
//  invert(&o->T[0][0],&o->Tinv[0][0]);
//  insertObject(o,&object_list);

//  o=newPlane(1.0,0.0,0.0,.25,.25,.75,.05,1.4);		// Right
//  Scale(o,10,10,10);
//  RotateZ(o,PI/2);
//  Translate(o,5,0,0);
//  invert(&o->T[0][0],&o->Tinv[0][0]);
//  insertObject(o,&object_list);

//   o=newPlane(1.0,0.0,0.0,.75,.75,.75,.05,1.4);		// Back
//   Scale(o,10,10,10);
//  Translate(o,0,0,10);
//  invert(&o->T[0][0],&o->Tinv[0][0]);
//  insertObject(o,&object_list);

//  o=newPlane(1.0,0.0,0.0,.75,.75,.75,.02,1.4);	// Bottom
//  Scale(o,10,10,10);
//  RotateX(o,PI/2);
//  Translate(o,0,-10,0);
//  loadTexture(o,"./texture/grass.ppm",1,&texture_list);
//  loadTexture(o,"./texture/ngrass.ppm",2,&texture_list);
//  invert(&o->T[0][0],&o->Tinv[0][0]);
//  insertObject(o,&object_list);

//   o=newPlane(1.0,0.0,0.0,.75,.75,.75,.05,1.4);		// Top
//   Scale(o,10,10,10);
//   RotateX(o,PI/2);
//   Translate(o,0,10,0);
//  invert(&o->T[0][0],&o->Tinv[0][0]);
//  insertObject(o,&object_list);

    // Cornell box
    o = newPlane(1.0, 0.0, 0.0, .75, .75, .75, .02, 1.4);    // Left
    Scale(o, 20, 20, 20);
// RotateX(o,PI/2);
// Translate(o,0,-10, 10);
    RotateY(o, PI / 2);
    Translate(o, -14, 0, 10);
    loadTexture(o, "./texture/background.ppm", 1, &texture_list);
    loadTexture(o, "./texture/nbackground.ppm", 2, &texture_list);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);

    o = newPlane(1.0, 0.0, 0.0, .75, .75, .75, .02, 1.4);    // Right
    Scale(o, 20, 20, 20);
// RotateX(o,PI/2);
// Translate(o,0,-10, 10);
    RotateY(o, PI / 2);
    Translate(o, 14, 0, 10);
    loadTexture(o, "./texture/background.ppm", 1, &texture_list);
    loadTexture(o, "./texture/nbackground.ppm", 2, &texture_list);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);


    o = newPlane(1.0, 0.0, 0.0, .75, .75, .75, .05, 1.4);        // Back
    Scale(o, 15, 12, 12);
    RotateZ(o, PI);
    Translate(o, 0, 0, 20);
    loadTexture(o, "./texture/arcDoor.ppm", 1, &texture_list);
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
    loadTexture(o, "./texture/floor.ppm", 1, &texture_list);
    loadTexture(o, "./texture/nfloor.ppm", 2, &texture_list);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);

    o = newSphere(1.0, 0.0, 0.0, .75, .75, .75, .05, 1.4);        // Top
    Scale(o, 500, 500, 500);
    Translate(o, 0, 510, 5);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);

//  Two spheres scene
//  o=newSphere(0.0,0.0,1.0,.99,.99,.99,.01,1.54);		// Refract
//  Scale(o,1.5,1.5,1.5);
//  Translate(o,0,-7.5,4);
//  invert(&o->T[0][0],&o->Tinv[0][0]);
//  insertObject(o,&object_list);

    double T[4][4] = {{1.0, 0.0, 0.0, 0.0},
                      {0.0, 1.0, 0.0, 0.0},
                      {0.0, 0.0, 1.0, 0.0},
                      {0.0, 0.0, 0.0, 1.0}};
    ScaleMat(T, 1.5, 1.5, 1.5);
    TranslateMat(T, 0, -7.5, 4);

    hierarchical(T, 2, 0.0, 0.0, 1.0, .01, 1.54, 0, 1);

//  o=newSphere(0.0,1.0,0.0,.99,.99,.99,.05,2.47);		// Reflect
//  Scale(o,3.75,3.75,3.75);
//  Translate(o,4.5,-3.75,6.5);
//  invert(&o->T[0][0],&o->Tinv[0][0]);
//  insertObject(o,&object_list);

/*
 // Ring of refracting spheres
 for (int i=0; i<5;i++)
 {
  o=newSphere(0.0,0.0,1.0,.99,.99,.99,.01,1.45+(.1*i));
  Scale(o,1.75,1.75,1.75);
  Translate(o,3.25*cos(2*PI*i/5),-2.45,3+3.25*sin(2*PI*i/5));
  invert(&o->T[0][0],&o->Tinv[0][0]);
  insertObject(o,&object_list);
 }

 for (int i=0; i<7;i++)
 {
  o=newSphere(0.0,0.0,1.0,.99,.99,.99,.01,2.00+(.05*i));
  Scale(o,1.75,1.75,1.75);
  Translate(o,4.60*cos(2*PI*i/7),-6.35,3+4.60*sin(2*PI*i/7));
  invert(&o->T[0][0],&o->Tinv[0][0]);
  insertObject(o,&object_list);
 }
*/




    o = newCyl(1.0, 0.0, 0.0, .99, .99, .99, .01, 1.54);
    Scale(o, 0.5, 0.5, 20);
    RotateX(o, PI / 2);
    Translate(o, -7, -2, 5.5);
// RotateY(o, PI/2.5);
// Translate(o,-3.9,0,0);
    loadTexture(o, "./texture/cyl1.ppm", 1, &texture_list);
    loadTexture(o, "./texture/ncyl1.ppm", 2, &texture_list);
//Scale(o,1.5,1,1);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);


    double M[4][4] = {{1.0, 0.0, 0.0, 0.0},
                      {0.0, 1.0, 0.0, 0.0},
                      {0.0, 0.0, 1.0, 0.0},
                      {0.0, 0.0, 0.0, 1.0}};
    ScaleMat(M, .7, .7, .7);
    TranslateMat(M, -8.5, 6, 4);

    hierarchical(M, 27, 0.0, 1.0, 0.0, .01, 1.54, 0, 0);


    o = newCyl(1.0, 0.0, 0.0, .99, .99, .99, .01, 1.54);
    Scale(o, 0.5, 0.5, 20);
    RotateX(o, PI / 2);
    Translate(o, 7, -2, 5.5);
// RotateY(o, PI/2.5);
// Translate(o,-3.9,0,0);
    loadTexture(o, "./texture/cyl2.ppm", 1, &texture_list);
    loadTexture(o, "./texture/ncyl2.ppm", 2, &texture_list);
//Scale(o,1.5,1,1);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    insertObject(o, &object_list);


    double M2[4][4] = {{1.0, 0.0, 0.0, 0.0},
                       {0.0, 1.0, 0.0, 0.0},
                       {0.0, 0.0, 1.0, 0.0},
                       {0.0, 0.0, 0.0, 1.0}};
    ScaleMat(M2, .7, .7, .7);
    TranslateMat(M2, 8.5, 6, 4);

    hierarchical(M2, 27, 0.0, 1.0, 0.0, .01, 1.54, 1, 0);




    // Planar light source at top
    o = newPlane(1.00, 0.00, 0.0, 1.0, 1.0, 1.0, 0.0, 1.54);
    Scale(o, 2, 8, 3);
    RotateX(o, PI / 2);
    Translate(o, 0, 10, 9);
    invert(&o->T[0][0], &o->Tinv[0][0]);
    o->isLightSource = 1;
    insertObject(o, &object_list);
}

void hierarchical(double M1[4][4], double depth, double diffPct, double reflPct, double tranPct, double refl_sig,
                  double r_index, double orientaion, double loop) {
    if (depth > 0) {
        if (loop) {
            struct object3D *o = newSphere(diffPct, reflPct, tranPct, .99, .99, .99, refl_sig, r_index);
            memcpy(o->T, M1, 16 * sizeof(double));
            invert(&o->T[0][0], &o->Tinv[0][0]);
            insertObject(o, &object_list);
            for (int i = 0; i < 2; i++) {
                double local[4][4] = {{1.0, 0.0, 0.0, 0.0},
                                      {0.0, 1.0, 0.0, 0.0},
                                      {0.0, 0.0, 1.0, 0.0},
                                      {0.0, 0.0, 0.0, 1.0}};
                ScaleMat(local, .75, .75, .75);
                TranslateMat(local, 0, 1.6, 0);
                RotateZMat(local, PI / 3 * pow(-1, i));

                matMult(M1, local);
                hierarchical(local, depth - 1, diffPct, 1 - reflPct, 1 - tranPct, refl_sig, r_index, orientaion, loop);
            }
        } else {
            struct object3D *o = newSphere(diffPct, reflPct, tranPct, drand48(), drand48(), drand48(), refl_sig,
                                           r_index);
            memcpy(o->T, M1, 16 * sizeof(double));
            invert(&o->T[0][0], &o->Tinv[0][0]);
            insertObject(o, &object_list);

            double local[4][4] = {{1.0, 0.0, 0.0, 0.0},
                                  {0.0, 1.0, 0.0, 0.0},
                                  {0.0, 0.0, 1.0, 0.0},
                                  {0.0, 0.0, 0.0, 1.0}};

            TranslateMat(local, -2.5 * pow(-1, orientaion), -0.75, 0);
            RotateYMat(local, PI / 5 * pow(-1, orientaion));
            matMult(M1, local);
            hierarchical(local, depth - 1, diffPct, reflPct, tranPct, refl_sig, r_index, orientaion, loop);
        }
    }
}