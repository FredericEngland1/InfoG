#version 460 core

#define EPSILON 0.0000001f

layout(rgba32f, binding = 1) readonly uniform image2D img_input;
layout(rgba32f, binding = 2) writeonly uniform image2D img_output;

uniform vec3 rOrigin;
uniform vec3 rVector;

void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 coordsTemp = ivec2(gl_GlobalInvocationID.xy);

    if (coords.x % 3 == 0) {

        vec3 triangleVector1 = vec3(imageLoad(img_input, coordsTemp));
        coordsTemp.x++;
        vec3 triangleVector2 = vec3(imageLoad(img_input, coordsTemp));
        coordsTemp.x++;
        vec3 triangleVector3 = vec3(imageLoad(img_input, coordsTemp));

        float det, invDet, u, v, t;

        vec3 h, e1, e2;

        e1 = triangleVector2 - triangleVector1;
        e2 = triangleVector3 - triangleVector1;

        h = cross(rVector, e2);
        det = dot(e1, h);

        // Check if triangle is parallel

        if (det < EPSILON) // If negatif then backfacing, if close to 0 then parallel
        {
            imageStore(img_output, coords, vec4(0, 0, 0, 0));
        }
        else {
            invDet = 1.0f / det;
            vec3 s = rOrigin - triangleVector1;
            u = dot(s, h) * invDet;

            if (u < 0.0 || u > 1.0) {
                imageStore(img_output, coords, vec4(0, 0, 0, 0));
            }
            else {
                vec3 q = cross(s, e1);
                v = invDet * dot(rVector, q);

                if (v < 0.0 || (u + v) > 1.0) {
                    imageStore(img_output, coords, vec4(0, 0, 0, 0));
                }
                else {
                    t = invDet * dot(e2, q);
                    //if (t <= EPSILON) return; // TODO useless ?

                    vec3 intersectionPoint = rOrigin + (rVector * t);

                    imageStore(img_output, coords, vec4(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z, t));
                }
            }
        }
    }
    else {
        imageStore(img_output, coords, vec4(0, 0, 0, 0));
    }
}