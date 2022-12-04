/*
  CSC D18 - Path Tracer code.

  Derived from the ray tracer starter code. Most function 
  names are identical, though in practice the implementation
  should be much simpler!

  You only need to modify or add code in sections
  clearly marked "TO DO" - remember to check what
  functionality is actually needed for the corresponding
  assignment!

  Last updated: Aug. 2017   - F.J.E.
*/

/*****************************************************************************
* COMPLETE THIS TEXT BOX:
*
* 1) Student Name: Dezhi Ren
* 2) Student Name:
*
* 1) Student number: 1005736795
* 2) Student number:
*
* 1) UtorID rendezhi
* 2) UtorID
*
* We hereby certify that the work contained here is our own
*
* ___Ren, Dezhi_____             _____________________
* (sign with your name)            (sign with your name)
********************************************************************************/

#include "utils_path.h"            // <-- This includes PathTracer.h

#define __USE_IS            // Use importance sampling for diffuse materials
#define __USE_ES            // Use explicit light sampling
// #define __DEBUG			// <-- Use this to turn on/off debugging output

// A couple of global structures and data: An object list, a light list, and the
// maximum recursion depth
struct object3D *object_list;
struct textureNode *texture_list;
unsigned long int NUM_RAYS;
int LS_NUM;
int MAX_DEPTH;

#include "buildScene.c"            // Import scene definition

void findFirstHit(struct ray3D *ray, double *lambda, struct object3D *Os, struct object3D **obj, struct point3D *p,
                  struct point3D *n, double *a, double *b) {
    // Find the closest intersection between the ray and any objects in the scene.
    // Inputs:
    //   *ray    -  A pointer to the ray being traced
    //   *Os     -  'Object source' is a pointer toward the object from which the ray originates. It is used for reflected or refracted rays
    //              so that you can check for and ignore self-intersections as needed. It is NULL for rays originating at the center of
    //              projection
    // Outputs:
    //   *lambda -  A pointer toward a double variable 'lambda' used to return the lambda at the intersection point
    //   **obj   -  A pointer toward an (object3D *) variable so you can return a pointer to the object that has the closest intersection with
    //              this ray (this is required so you can do the shading)
    //   *p      -  A pointer to a 3D point structure so you can store the coordinates of the intersection point
    //   *n      -  A pointer to a 3D point structure so you can return the normal at the intersection point
    //   *a, *b  -  Pointers toward double variables so you can return the texture coordinates a,b at the intersection point

    /////////////////////////////////////////////////////////////
    // TO DO: Implement this function. See the notes for
    // reference of what to do in here
    /////////////////////////////////////////////////////////////
    double lambda_min = -1.0;
    *lambda = -1.0;

    (ray->rayPos)(ray, .0000000001, &ray->p0);
    // traverse all the object that is not itself or light source
    for (struct object3D *i = object_list; i != NULL; i = i->next) {
        if (i != Os) {
            struct point3D tmp_p, tmp_n;
            double tmp_a, tmp_b;

            //find intersection
            (i->intersect)(i, ray, &lambda_min, &tmp_p, &tmp_n, &tmp_a, &tmp_b);

            // replace first hit with the smallest positive lambda
            if (lambda_min > 0 && (lambda_min < *lambda || *lambda <= 0)) {
                *lambda = lambda_min;
                memcpy(p, &tmp_p, sizeof(struct point3D));
                memcpy(n, &tmp_n, sizeof(struct point3D));
                *a = tmp_a;
                *b = tmp_b;
                *obj = i;
            }
        }
    }
}

void PathTrace(struct ray3D *ray, int depth, struct colourRGB *col, struct object3D *Os, int CEL) {
    // Trace one light path through the scene.
    //
    // Parameters:
    //   *ray   -  A pointer to the ray being traced
    //   depth  -  Current recursion depth for recursive raytracing
    //   *col   - Pointer to an RGB colour structure so you can return the object colour
    //            at the intersection point of this ray with the closest scene object.
    //   *Os    - 'Object source' is a pointer to the object from which the ray
    //            originates so you can discard self-intersections due to numerical
    //            errors. NULL for rays originating from the center of projection.
    // CEL - can tell the recursive call whether or not the explicit light ray from the current call hit the lightsource.
//        Use it to decide whether to accumulate light or not if the recursive call hits the lightsource

    double lambda;            // Lambda at intersection
    double a, b;            // Texture coordinates
    struct object3D *obj;        // Pointer to object at intersection
    struct point3D p;        // Intersection point
    struct point3D n;        // Normal at intersection
    double R, G, B;            // Handy in case you need to keep track of some RGB colour value
    double dice;            // Handy to keep a random value

    double max_num = max(ray->R, ray->G);
    max_num = max(max_num, ray->B);
    dice = 0.5 * drand48();
    //!!!
    // dice = drand48();
    //double temp_double = (1 - (ray->R + ray->G + ray->B)/3) / MAX_DEPTH; // roll dice to kill
    // Max recursion depth reached. Return black (no light coming into pixel from this path).
    // or not pass Russian Roulette check
    if (max_num < dice || depth > MAX_DEPTH) {
#ifdef __USE_ES
        col->R = ray->Ir;    // These are accumulators, initialized at 0. Whenever we find a source of light these
        col->G = ray->Ig;    // get incremented accordingly. At the end of the recursion, we return whatever light
        col->B = ray->Ib;    // we accumulated into these three values.
#else
        col->R = 0;
        col->G = 0;
        col->B = 0;
#endif
        return;
    } else {
        ///////////////////////////////////////////////////////
        // TO DO: Complete this function. Refer to the notes
        // if you are unsure what to do here.
        ///////////////////////////////////////////////////////
        findFirstHit(ray, &lambda, Os, &obj, &p, &n, &a, &b);

        if (lambda > 0) {
            if (obj->texImg == NULL)        // Not textured, use object colour
            {
                R = obj->col.R;
                G = obj->col.G;
                B = obj->col.B;
            } else {
                // Get object colour from the texture given the texture coordinates (a,b), and the texturing function
                // for the object. Note that we will use textures also for Photon Mapping.
                obj->textureMap(obj->texImg, a, b, &R, &G, &B);
            }

            if (!obj->isLightSource) {

                dice = drand48();
                double diffPct = obj->diffPct;
                double reflPct = obj->reflPct;
                // 0 | diffusion | reflection | refraction | 1
                // diffusion      dice < diffPct
                // reflection     diffPct <= dice < diffPct + reflPct
                // refraction     diffPct + reflPct <= dice < 1

                ray->R *= R;
                ray->G *= G;
                ray->B *= B;
                struct point3D new_dirc;
                if (dice < diffPct) {
#ifdef __USE_ES
                    // get a random light source
                    double dice2 = drand48();
                    struct object3D *chosen_LS;
                    double acc = 0;
                    for (chosen_LS = object_list; chosen_LS != NULL ; chosen_LS = chosen_LS->next) {
                        if (chosen_LS->isLightSource) {
                            acc += chosen_LS->LSweight;
                            if (dice2 <= acc) break;
                        }
                    }

                    // generate a random ray from the point to the light source
                    struct ray3D *ray_explict = newRay(&p, &ray->d);
                    double x_es, y_es, z_es;
                    (chosen_LS->randomPoint)(chosen_LS, &x_es, &y_es, &z_es);
                    ray_explict->d.px = x_es - p.px;
                    ray_explict->d.py = y_es - p.py;
                    ray_explict->d.pz = z_es - p.pz;
                    normalize(&ray_explict->d);

                    // if the light source is not behind the surface
                    if (dot(&ray_explict->d, &n) > 0) {
                        double lambda_ex;
                        double a_ex, b_ex;
                        struct object3D *tmp_ex;
                        struct point3D p_ex, n_ex;
                        findFirstHit(ray_explict, &lambda_ex, obj, &tmp_ex, &p_ex, &n_ex, &a_ex, &b_ex);

                        if (tmp_ex == chosen_LS) {
                            // mark this light source
                            CEL = chosen_LS->LSpointer;

                            // w = min(1, A*(n*l)(n_ls*-l)/d^2)
                            double d_sqr = pow(x_es - p.px, 2) + pow(y_es - p.py, 2) + pow(z_es - p.pz, 2);
                            double weight = 2 * PI * chosen_LS->LSweight * dot(&n, &ray_explict->d) *
                                    -dot(&n_ex, &ray_explict->d) / d_sqr;
                            weight = min(1, weight);
                            ray->Ir += ray->R * chosen_LS->col.R * weight;
                            ray->Ig += ray->G * chosen_LS->col.G * weight;
                            ray->Ib += ray->B * chosen_LS->col.B * weight;
                        } else {
                            CEL = 0;
                        }
                    }
                    free(ray_explict);
#endif

#ifdef __USE_IS
                    cosWeightedSample(&n, &new_dirc);
#else
                    getRandomDirection(&new_dirc);
#endif
                    double dn = dot(&n, &new_dirc);
                    ray->R *= dn;
                    ray->G *= dn;
                    ray->B *= dn;

                } else {
                    // hit an reflective/refractive surface
                    getMirrorDirection(&new_dirc, ray, &n);

                    // disturb the normal by normal-distribution
                    new_dirc.px += box_muller() * obj->refl_sig;
                    new_dirc.py += box_muller() * obj->refl_sig;
                    new_dirc.pz += box_muller() * obj->refl_sig;

                    if (dice >= diffPct + reflPct) {
                        double n1 = ray->insideOut ? 1.0 : obj->r_index;
                        double n2 = ray->insideOut ? obj->r_index : 1.0;
                        double cos_theta1 = dot(&ray->d, &n);
                        double sin_theta1 = sqrt(1 - pow(cos_theta1, 2));
                        double r0 = pow((n1 - n2) / (n1 + n2), 2);
                        double reflectance = r0 + (1 - r0) * pow((1 + cos_theta1), 5);
                        double sin_theta2 = (double) (n1 / n2) * sin_theta1;

                        if (drand48()> reflectance && sin_theta2 < 1 && sin_theta2 > 0) {
                            double temp1 = n1 / n2;
                            double temp2 = -dot(&n, &ray->d);
                            double temp3 = temp1 * temp2 - sqrt(1 - temp1 * temp1 * (1 - temp2 * temp2));

                            new_dirc.px = temp1 * ray->d.px + temp3 * n.px;
                            new_dirc.py = temp1 * ray->d.py + temp3 * n.py;
                            new_dirc.pz = temp1 * ray->d.pz + temp3 * n.pz;

                            ray->insideOut = 1 - ray->insideOut;
                        }
                        obj = NULL;
                    }
                }

                normalize(&new_dirc);
                struct ray3D *ray_reflect = newRay(&p, &new_dirc);
                ray_reflect->srcN = n;
                ray_reflect->insideOut = ray->insideOut;

                ray_reflect->R = ray->R;
                ray_reflect->G = ray->G;
                ray_reflect->B = ray->B;
#ifdef __USE_ES
                ray_reflect->Ir = ray->Ir;
                ray_reflect->Ig = ray->Ig;
                ray_reflect->Ib = ray->Ib;
#endif

                PathTrace(ray_reflect, depth + 1, col, obj, CEL);

                free(ray_reflect);

            } else {
                //hit the lightsource
                col->R = ray->Ir;
                col->G = ray->Ig;
                col->B = ray->Ib;
#ifdef __USE_ES
                if (CEL != obj->LSpointer) {
                    col->R += ray->R * R;
                    col->G += ray->G * G;
                    col->B += ray->B * B;
                }

#else
                col->R += ray->R * R;
                col->G += ray->G * G;
                col->B += ray->B * B;
#endif
                col->R = min(1, col->R);
                col->G = min(1, col->G);
                col->B = min(1, col->B);
                return;
            }
        } else {
#ifdef __USE_ES
            col->R = ray->Ir;
            col->G = ray->Ig;
            col->B = ray->Ib;
#else
            col->R = 0;
            col->G = 0;
            col->B = 0;
#endif

            return;
        }
    }

}

int main(int argc, char *argv[]) {
    // Main function for the path tracer. Parses input parameters,
    // sets up the initial blank image, and calls the functions
    // that set up the scene and do the raytracing.
    struct image *im;        // Will hold the final image
    struct view *cam;        // Camera and view for this scene
    int sx;            // Size of the  image
    int num_samples;        // Number of samples to use per pixel
    char output_name[1024];    // Name of the output file for the .ppm image file
    struct point3D e;        // Camera view parameters 'e', 'g', and 'up'
    struct point3D g;
    struct point3D up;
    double du, dv;            // Increase along u and v directions for pixel coordinates
    struct point3D pc, d;        // Point structures to keep the coordinates of a pixel and
    // the direction or a ray
    struct ray3D *ray;        // Structure to keep the ray from e to a pixel
    struct colourRGB col;        // Return colour for pixels
    int i, j, k;            // Counters for pixel coordinates and samples
    double *rgbIm;            // Image is now double precision floating point since we
    // will be accumulating brightness differences with a
    // wide dynamic range
    struct object3D *obj;        // Will need this to process lightsource weights
    double *wght;            // Holds weights for each pixel - to provide log response
    double pct, wt;

    time_t t1, t2;
    FILE *f;

    if (argc < 5) {
        fprintf(stderr, "PathTracer: Can not parse input parameters\n");
        fprintf(stderr, "USAGE: PathTracer size rec_depth num_samples output_name\n");
        fprintf(stderr, "   size = Image size (both along x and y)\n");
        fprintf(stderr, "   rec_depth = Recursion depth\n");
        fprintf(stderr, "   num_samples = Number of samples per pixel\n");
        fprintf(stderr, "   output_name = Name of the output file, e.g. MyRender.ppm\n");
        exit(0);
    }
    sx = atoi(argv[1]);
    MAX_DEPTH = atoi(argv[2]);
    num_samples = atoi(argv[3]);
    strcpy(&output_name[0], argv[4]);

    fprintf(stderr, "Rendering image at %d x %d\n", sx, sx);
    fprintf(stderr, "Recursion depth = %d\n", MAX_DEPTH);
    fprintf(stderr, "NUmber of samples = %d\n", num_samples);
    fprintf(stderr, "Output file name: %s\n", output_name);

    object_list = NULL;
    texture_list = NULL;

    // Allocate memory for the new image
    im = newImage(sx, sx);
    wght = (double *) calloc(sx * sx, sizeof(double));
    if (!im || !wght) {
        fprintf(stderr, "Unable to allocate memory for image\n");
        exit(0);
    } else rgbIm = (double *) im->rgbdata;
    for (i = 0; i < sx * sx; i++) *(wght + i) = 1.0;

    buildScene();        // Create a scene.

    // Mind the homogeneous coordinate w of all vectors below. DO NOT
    // forget to set it to 1, or you'll get junk out of the
    // geometric transformations later on.

    // Camera center
    e.px = 0;
    e.py = 0;
    e.pz = -15;
    e.pw = 1;

    // To define the gaze vector, we choose a point 'pc' in the scene that
    // the camera is looking at, and do the vector subtraction pc-e.
    // Here we set up the camera to be looking at the origin.
    g.px = 0 - e.px;
    g.py = 0 - e.py;
    g.pz = 0 - e.pz;
    g.pw = 1;
    // In this case, the camera is looking along the world Z axis, so
    // vector w should end up being [0, 0, -1]

    // Define the 'up' vector to be the Y axis
    up.px = 0;
    up.py = 1;
    up.pz = 0;
    up.pw = 1;

    // Set up view with given the above vectors, a 4x4 window,
    // and a focal length of -1 (why? where is the image plane?)
    // Note that the top-left corner of the window is at (-2, 2)
    // in camera coordinates.
    cam = setupView(&e, &g, &up, -3, -2, 2, 4);

    if (cam == NULL) {
        fprintf(stderr, "Unable to set up the view and camera parameters. Our of memory!\n");
        cleanup(object_list, texture_list);
        deleteImage(im);
        exit(0);
    }

    du = cam->wsize / (sx - 1);        // du and dv. In the notes in terms of wl and wr, wt and wb,
    dv = -cam->wsize / (sx - 1);        // here we use wl, wt, and wsize. du=dv since the image is
    // and dv is negative since y increases downward in pixel
    // coordinates and upward in camera coordinates.

    fprintf(stderr, "View parameters:\n");
    fprintf(stderr, "Left=%f, Top=%f, Width=%f, f=%f\n", cam->wl, cam->wt, cam->wsize, cam->f);
    fprintf(stderr, "Camera to world conversion matrix (make sure it makes sense!):\n");
    printmatrix(cam->C2W);
    fprintf(stderr, "World to camera conversion matrix:\n");
    printmatrix(cam->W2C);
    fprintf(stderr, "\n");

    LS_NUM = 0;
    // Update light source weights - will give you weights for each light source that add up to 1
    obj = object_list;
    pct = 0;
    while (obj != NULL) {
        if (obj->isLightSource)
            pct += obj->LSweight;
        obj = obj->next;
    }
    obj = object_list;
    while (obj != NULL) {
        if (obj->isLightSource) {
            obj->LSweight /= pct;
            LS_NUM += 1;
            obj->LSpointer = LS_NUM;
        }
        obj = obj->next;
    }
    fprintf(stderr, "\n");

    NUM_RAYS = 0;

    t1 = time(NULL);

    fprintf(stderr, "Rendering pass... ");
    for (k = 0; k < num_samples; k++) {
        fprintf(stderr, "%d/%d, ", k, num_samples);
#pragma omp parallel for schedule(dynamic, 1) private(i, j, pc, wt, ray, col, d)
        for (j = 0; j < sx; j++)        // For each of the pixels in the image
        {
            for (i = 0; i < sx; i++) {
                // Random sample within the pixel's area
                pc.px = (cam->wl + ((i + (drand48() - .5)) * du));
                pc.py = (cam->wt + ((j + (drand48() - .5)) * dv));
                pc.pz = cam->f;
                pc.pw = 1;

                // Convert image plane sample coordinates to world coordinates
                matVecMult(cam->C2W, &pc);

                // Now compute the ray direction
                memcpy(&d, &pc, sizeof(struct point3D));
                subVectors(&cam->e, &d);        // Direction is d=pc-e
                normalize(&d);

                // Create a ray and do the raytracing for this pixel.
                ray = newRay(&pc, &d);

                if (ray != NULL) {
                    wt = *(wght + i + (j * sx));
                    PathTrace(ray, 1, &col, NULL, 0);
                    (*(rgbIm + ((i + (j * sx)) * 3) + 0)) += col.R * pow(2, -log(wt));
                    (*(rgbIm + ((i + (j * sx)) * 3) + 1)) += col.G * pow(2, -log(wt));
                    (*(rgbIm + ((i + (j * sx)) * 3) + 2)) += col.B * pow(2, -log(wt));
                    wt += col.R;
                    wt += col.G;
                    wt += col.B;
                    *(wght + i + (j * sx)) = wt;
                    free(ray);
                }
            } // end for i
        } // end for j
        if (k % 25 == 0) dataOutput(rgbIm, sx, &output_name[0]);        // Update output image every 25 passes
    } // End for k
    t2 = time(NULL);

    fprintf(stderr, "\nDone!\n");

    dataOutput(rgbIm, sx, &output_name[0]);

    fprintf(stderr, "Total number of rays created: %ld\n", NUM_RAYS);
    fprintf(stderr, "Rays per second: %f\n", (double) NUM_RAYS / (double) difftime(t2, t1));

    // Exit section. Clean up and return.
    cleanup(object_list, texture_list);            // Object and texture lists
    deleteImage(im);                    // Rendered image
    free(cam);                        // camera view
    free(wght);
    exit(0);
}