#include "coll.h"
#include "../cutil.h"

#include <string.h>
#include <float.h>
#include <math.h>

void
_CONV_topleftify(
    eau_rect* rect
    ) {
    float offx; float offy;

    switch (rect->align) {
    case EAU_ALIGN_TOP_LEFT:  offx = 0;  offy = 0;  break;
    case EAU_ALIGN_TOP:       offx = .5; offy = 0;  break;
    case EAU_ALIGN_TOP_RIGHT: offx = 1;  offy = 0;  break;
    case EAU_ALIGN_MID_LEFT:  offx = 0;  offy = .5; break;
    case EAU_ALIGN_MID:       offx = .5; offy = .5; break;
    case EAU_ALIGN_MID_RIGHT: offx = 1;  offy = .5; break;
    case EAU_ALIGN_BOT_LEFT:  offx = 0;  offy = 1;  break;
    case EAU_ALIGN_BOT:       offx = .5; offy = 1;  break;
    case EAU_ALIGN_BOT_RIGHT: offx = 1;  offy = 1;  break;
    }

    rect->x -= rect->w * offx;
    rect->y -= rect->h * offy;
    rect->align = EAU_ALIGN_TOP_LEFT;
}

bool
eau_aabb2d(
    float min1x, float min1y,
    float max1x, float max1y,
    float min2x, float min2y,
    float max2x, float max2y
    ) {
    return min1x < max2x &&
           max1x > min2x &&
           min1y < max2y &&
           max1y > min2y;
}

bool
eau_point_aabb2d(
    float pointx, float pointy,
    float minx, float miny,
    float maxx, float maxy
    ) {
    return pointx < maxx &&
           pointx > minx &&
           pointy < maxy &&
           pointy > miny;
}

bool
eau_rect_rect(
    eau_rect a,
    eau_rect b
    ) {
    _CONV_topleftify(&a);
    _CONV_topleftify(&b);

    return eau_aabb2d(a.x,a.y, a.x+a.w,a.y+a.h, b.x,b.y, b.x+b.w,b.y+b.h);
}

bool
eau_point_rect(
    float pointx, float pointy,
    eau_rect rect
    ) {
    _CONV_topleftify(&rect);

    return eau_point_aabb2d(pointx, pointy, rect.x,rect.y, rect.x+rect.w,rect.y+rect.h);
}

bool
eau_aabb3d(
    float min1x, float min1y, float min1z,
    float max1x, float max1y, float max1z,
    float min2x, float min2y, float min2z,
    float max2x, float max2y, float max2z
    ) {
    return min1x < max2x &&
           max1x > min2x &&
           min1y < max2y &&
           max1y > min2y &&
           min1z < max2z &&
           max1z > min2z;
}


float
_eau_dot(
    float a[3],
    float b[3]
    ) {
    float res = 0;
    for (int i = 0; i < 3; ++i)
        res += a[i]*b[i];
    return res;
}

void
_eau_cross(
    float (*out)[3],
    float a[3],
    float b[3]
    ) {
    *out[0] = a[1] * b[2] - a[2] * b[1];
    *out[1] = a[2] * b[0] - a[0] * b[2];
    *out[2] = a[0] * b[1] - a[1] * b[0];
}

void 
_eau_furthest(
    float hull[][3], uint32_t hull_verts,
    float dir[3],
    float (*max)[3]
    ) {
    float maxdist = FLT_MIN;
    for (int i = 0; i < hull_verts; ++i) {
        float vert[3] = { hull[i][0], hull[i][1], hull[i][2] };
        float dist = _eau_dot(vert, dir);
        if (dist > maxdist) {
            maxdist = dist;
            *max[0] = vert[0];
            *max[1] = vert[1];
            *max[2] = vert[2];
        }
    }
}

void
_eau_sub_vec3(
    float (*res)[3],
    float a[3],
    float b[3]
    ) {
    *res[0] = a[0] - b[0];
    *res[1] = a[1] - b[1];
    *res[2] = a[2] - b[2];
}

void
_eau_support(
    float hull1[][3], uint32_t hull1_verts,
    float hull2[][3], uint32_t hull2_verts,
    float dir[3],
    float (*max)[3]
    ) {
    float _max[3];
    _eau_furthest(hull1, hull1_verts, dir, &_max);
    _eau_furthest(hull2, hull2_verts, (float[3]){ -dir[0], -dir[1], -dir[2] }, max);
    _eau_sub_vec3(max, _max, *max);
}

bool
_eau_same_dir(
    float dir[3],
    float ao[3] // OwO
    ) {
    return _eau_dot(dir, ao) > 0;
}

bool
_eau_line(
    float points[4][3],
    uint32_t* point_amt,
    float (*dir)[3]
    ) {
    eat_assert(*point_amt == 2, "HELP MEEE");

    float a[3]; float b[3];
    memcpy(a, points[0], sizeof(float)*3);
    memcpy(b, points[1], sizeof(float)*3);

    float ab[3]; float ao[3];
    _eau_sub_vec3(&ab, b,a);
    _eau_sub_vec3(&ao, (float[3]){0}, a);

    if (_eau_same_dir(ab, ao)) {
        _eau_cross(dir, ab, ao);
        _eau_cross(dir, *dir, ab);
    } else {
        memcpy(points[0], a, sizeof(float)*3);
        *point_amt = 1;
        memcpy(*dir, ao, sizeof(float)*3);
    }

    return false;
}

bool
_eau_tri(
    float points[4][3],
    uint32_t* point_amt,
    float (*dir)[3]
    ) {
    eat_assert(*point_amt == 3, "HELP MEEE");

    float a[3]; float b[3]; float c[3];
    memcpy(a, points[0], sizeof(float)*3);
    memcpy(b, points[1], sizeof(float)*3);
    memcpy(c, points[2], sizeof(float)*3);

    float ab[3]; float ac[3]; float ao[3];
    _eau_sub_vec3(&ab, b,a);
    _eau_sub_vec3(&ac, c,a);
    _eau_sub_vec3(&ao, (float[3]){0}, a);

    float abc[3];
    _eau_cross(&abc, ab, ac);

    float TEMP[3];

    _eau_cross(&TEMP, abc, ac);
    if (_eau_same_dir(TEMP, ao)) {
        if (_eau_same_dir(ac, ao)) {
            memcpy(points[0], a, sizeof(float)*3);
            memcpy(points[1], c, sizeof(float)*3);
            *point_amt = 2;
            _eau_cross(dir, ac, ao);
            _eau_cross(dir, *dir, ac);
        } else {
            memcpy(points[0], a, sizeof(float)*3);
            memcpy(points[1], b, sizeof(float)*3);
            *point_amt = 2;
            return _eau_line(points, point_amt, dir);
        }
    } else {
        _eau_cross(&TEMP, ab, abc);
        if (_eau_same_dir(TEMP, ao)) {
            memcpy(points[0], a, sizeof(float)*3);
            memcpy(points[1], b, sizeof(float)*3);
            *point_amt = 2;
            return _eau_line(points, point_amt, dir);
        } else {
            if (_eau_same_dir(abc, ao)) memcpy(*dir, abc, sizeof(float)*3);
            else {
                memcpy(points[0], a, sizeof(float)*3);
                memcpy(points[1], c, sizeof(float)*3);
                memcpy(points[2], b, sizeof(float)*3);
                memcpy(*dir, (float[3]){ -abc[0], -abc[1], -abc[2] }, sizeof(float)*3);
            }
        }
    }

    return false;
}

bool
_eau_tetra(
    float points[4][3],
    uint32_t* point_amt,
    float (*dir)[3]
    ) {
    eat_assert(*point_amt == 4, "HELP MEEE");

    float a[3]; float b[3]; float c[3]; float d[3];
    memcpy(a, points[0], sizeof(float)*3);
    memcpy(b, points[1], sizeof(float)*3);
    memcpy(c, points[2], sizeof(float)*3);
    memcpy(d, points[4], sizeof(float)*3);

    float ab[3]; float ac[3]; float ad[3]; float ao[3];
    _eau_sub_vec3(&ab, b,a);
    _eau_sub_vec3(&ac, c,a);
    _eau_sub_vec3(&ad, d,a);
    _eau_sub_vec3(&ao, (float[3]){0}, a);

    float abc[3]; float acd[3]; float adb[3];
    _eau_cross(&abc, ab,ac);
    _eau_cross(&acd, ac,ad);
    _eau_cross(&adb, ad,ab);

    if (_eau_same_dir(abc, ao)) {
        memcpy(points[0], a, sizeof(float)*3);
        memcpy(points[1], b, sizeof(float)*3);
        memcpy(points[2], c, sizeof(float)*3);
        *point_amt = 3;
        return _eau_tri(points, point_amt, dir);
    } if (_eau_same_dir(acd, ao)) {
        memcpy(points[0], a, sizeof(float)*3);
        memcpy(points[1], c, sizeof(float)*3);
        memcpy(points[2], d, sizeof(float)*3);
        *point_amt = 3;
        return _eau_tri(points, point_amt, dir);
    } if (_eau_same_dir(adb, ao)) {
        memcpy(points[0], a, sizeof(float)*3);
        memcpy(points[1], d, sizeof(float)*3);
        memcpy(points[2], b, sizeof(float)*3);
        *point_amt = 3;
        return _eau_tri(points, point_amt, dir);
    }

    return true;
}

bool
_eau_next_simplex(
    float points[4][3],
    uint32_t* point_amt,
    float (*dir)[3]
    ) {
    switch (*point_amt) {
    case 2: return _eau_line(points, point_amt, dir);
    case 3: return _eau_tri(points, point_amt, dir);
    case 4: return _eau_tetra(points, point_amt, dir);
    }

    eat_unreachable();
}

void
_eau_normalize(
    float (*dir)[3]
    ) {
    float mag = *dir[0] * *dir[0];
    mag += *dir[1] * *dir[1];
    mag += *dir[2] * *dir[2];
    mag = sqrtf(mag);
    *dir[0] = *dir[0] / mag;
    *dir[1] = *dir[1] / mag;
    *dir[2] = *dir[2] / mag;
}

bool
eau_gjk3d(
    float hull1[][3], uint32_t hull1_verts,
    float hull2[][3], uint32_t hull2_verts,
    float (*out_simplex)[4][3]
    ) {
    float supp[3];
    _eau_support(hull1, hull1_verts, hull2, hull2_verts, (float[3]){ 1,0,0 }, &supp);

    float simplex[4][3];
    uint32_t simplex_points = 1;
    memcpy(simplex[0], supp, sizeof(float)*3);

    float dir[3] = { -supp[0], -supp[1], -supp[2] };

    for (int i = 0; i < 4; ++i) {
        _eau_normalize(&dir);
        _eau_support(hull1, hull1_verts, hull2, hull2_verts, dir, &supp);

        if (_eau_dot(supp, dir) <= 0) goto fail;

        memcpy(simplex[3], simplex[2], sizeof(float)*3);
        memcpy(simplex[2], simplex[1], sizeof(float)*3);
        memcpy(simplex[1], simplex[0], sizeof(float)*3);
        memcpy(simplex[0], supp, sizeof(float)*3);
        ++simplex_points;

        if (_eau_next_simplex(simplex, &simplex_points, &dir)) {
            memcpy(*out_simplex, simplex, sizeof(float)*12);
            return true;
        }
    }

fail:
    if (out_simplex) memcpy(*out_simplex, (float[4*3]){0}, sizeof(float)*12);
    return false;
}


void
_eau_get_face_normals(
    float (*polytope)[3], uint32_t polytope_size,
    uint32_t* faces, uint32_t face_amt,
    float (*out_norms)[4], uint32_t* out_norm_amt, uint32_t* out_tri 
    ) {
    out_norms = malloc(sizeof(float)*4);
    *out_norm_amt = 1;

    float mindist = FLT_MAX;

    for (uint32_t i = 0; i < face_amt; i += 3) {
        float a[3]; float b[3]; float c[3];
        memcpy(a, polytope[faces[i + 0]], sizeof(float)*3);
        memcpy(b, polytope[faces[i + 1]], sizeof(float)*3);
        memcpy(c, polytope[faces[i + 2]], sizeof(float)*3);

        float ab[3]; float ac[3];
        _eau_sub_vec3(&ab, b,a);
        _eau_sub_vec3(&ac, c,a);

        float norm[3];
        _eau_cross(&norm, ab, ac);
        _eau_normalize(&norm);

        float dist = _eau_dot(norm, a);
        if (dist < 0) {
            _eau_sub_vec3(&norm, (float[3]){0}, norm);
            dist = -dist;
        }

        ++*out_norm_amt;
        out_norms = realloc(out_norms, sizeof(float)*3 * *out_norm_amt);
        memcpy(out_norms[*out_norm_amt-1], (float[4]){ norm[0], norm[1], norm[2], dist }, sizeof(float)*4);

        if (dist < mindist) {
            *out_tri = i / 3;
            mindist = dist;
        }
    }
}

void
_eau_add_if_unique_edge(
    uint32_t (*edges)[2], uint32_t* edge_amt,
    uint32_t *faces, uint32_t face_amt,
    uint32_t a, uint32_t b
    ) {
    int32_t rev_ind = -1;
    for (uint32_t i = 0; i < *edge_amt; ++i) if (edges[i][0] == faces[b] && edges[i][1] == faces[a]) {
        rev_ind = i;
        break;
    }

    if (rev_ind != -1) {
        for (uint32_t i = rev_ind+1; i < *edge_amt; ++i)
            memcpy(edges[i-1], edges[i], sizeof(uint32_t)*2);
        --*edge_amt;
        edges = realloc(edges, sizeof(uint32_t) * *edge_amt);
    } else {
        ++*edge_amt;
        edges = realloc(edges, sizeof(uint32_t) * *edge_amt);
        memcpy(edges[*edge_amt-1], (uint32_t[2]){ faces[a], faces[b] }, sizeof(uint32_t)*2);
    }
}

eau_collision_info
eau_epa3d(
    float simplex[4][3],
    float hull1[][3], uint32_t hull1_verts,
    float hull2[][3], uint32_t hull2_verts
    ) {
    float (*polytope)[3] = malloc(sizeof(float)*3 *4);
    memcpy(polytope, simplex, sizeof(float)*3 *4);
    uint32_t polytope_size = 4;

    uint32_t* faces = malloc(sizeof(uint32_t) * 12);
    uint32_t face_amt = 12;
    memcpy(faces, (uint32_t[12]){
        0, 1, 2,
        0, 3, 1,
        0, 2, 3,
        1, 3, 2,
    }, sizeof(uint32_t) * 12);

    float (*normals)[4]; uint32_t normal_amt;
    uint32_t minface;
    _eau_get_face_normals(polytope, polytope_size, faces, face_amt, normals, &normal_amt, &minface);

    float minnorm[3];
    float mindist = FLT_MAX;

    for (uint32_t _i = 0; mindist == FLT_MAX && _i < 1024; ++_i) {
        memcpy(minnorm, normals[minface], sizeof(float)*3);
        mindist = normals[minface][3];

        float supp[3];
        _eau_support(hull1, hull1_verts, hull2, hull2_verts, minnorm, &supp);
        float sdist = _eau_dot(minnorm, supp);

        if (fabs(sdist - mindist) > .001f) {
            mindist = FLT_MAX;

            uint32_t (*unique)[2] = malloc(sizeof(uint32_t)*2);
            uint32_t unique_amt = 1;
            for (uint32_t i = 0; i < normal_amt; ++i) {
                float normsxyz[3];
                memcpy(normsxyz, normals[i], sizeof(float)*3);
                if (_eau_dot(normsxyz, supp) > _eau_dot(normsxyz, polytope[faces[i*3]])) {
                    uint32_t f = i * 3;
                    _eau_add_if_unique_edge(unique, &unique_amt, faces, face_amt, f+0, f+1);
                    _eau_add_if_unique_edge(unique, &unique_amt, faces, face_amt, f+1, f+2);
                    _eau_add_if_unique_edge(unique, &unique_amt, faces, face_amt, f+2, f+0);

                    faces[f+2] = faces[--face_amt];
                    faces[f+1] = faces[--face_amt];
                    faces[f+0] = faces[--face_amt];
                    faces = realloc(faces, sizeof(uint32_t) * face_amt);

                    memcpy(normals[i], normals[--normal_amt], sizeof(float)*4);
                    normals = realloc(normals, sizeof(float) * 4 * normal_amt);

                    --i;
                }
            }

            uint32_t* new_faces = malloc(sizeof(uint32_t) * unique_amt * 3);
            for (uint32_t i = 0; i < unique_amt; ++i) {
                new_faces[i*3 + 0] = unique[i][0];
                new_faces[i*3 + 1] = unique[i][1];
                new_faces[i*3 + 2] = polytope_size;
            }

            ++polytope_size;
            polytope = realloc(polytope, sizeof(float) * 3 * polytope_size);
            memcpy(polytope[polytope_size-1], supp, sizeof(float)*3);

            float (*new_normals)[4]; uint32_t new_normal_amt;
            uint32_t new_minface;
            _eau_get_face_normals(polytope, polytope_size, faces, face_amt, new_normals, &new_normal_amt, &new_minface);

            float old_mindist = FLT_MAX;
            for (uint32_t i = 0; i < normal_amt; ++i) if (normals[i][3] < old_mindist) {
                old_mindist = normals[i][3];
                minface = i;
            }

            if (new_normals[new_minface][3] < old_mindist) minface = new_minface + normal_amt;

            face_amt += unique_amt * 3;
            faces = realloc(faces, sizeof(uint32_t) * face_amt);
            normal_amt += new_normal_amt;
            normals = realloc(normals, sizeof(float)*4 * normal_amt);

            for (uint32_t i = 0; i < unique_amt * 3; ++i) faces[i + face_amt-unique_amt*3] = new_faces[i];
            for (uint32_t i = 0; i < new_normal_amt; ++i) memcpy(normals[i+new_normal_amt], new_normals[i], sizeof(float)*4);

            free(new_normals);
            free(new_faces);
            free(unique);
        }
    }

    free(normals);
    free(faces);
    free(polytope);

    return (eau_collision_info){
        .normx = minnorm[0],
        .normy = minnorm[1],
        .normz = minnorm[2],
        .depth = mindist,
    };
}
