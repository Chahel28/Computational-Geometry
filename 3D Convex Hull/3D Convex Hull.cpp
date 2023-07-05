#include <bits/stdc++.h>
#include <fstream>

using namespace std;

struct point {
    long double x, y, z;
    point(long double x, long double y, long double z) : x(x), y(y), z(z) {}
    point() : x(0), y(0), z(0) {}
};

struct face {
    int a, b, c; // indices of the points
    struct point normal;
    face (int a, int b, int c, struct point normal) : a(a), b(b), c(c), normal(normal) {}
};

vector<face> convex_hull;
vector<point> points;
vector<vector<bool>> in_convex_hull;
const long double epsilon = 0.00000001;
int no_of_points = 0;

struct point cross_product (struct point a, struct point b) {
    long double x = a.y * b.z - a.z * b.y;
    long double y = a.z * b.x - a.x * b.z;
    long double z = a.x * b.y - a.y * b.x;
    return point(x, y, z);
}

struct point subtract (struct point a, struct point b) {
    long double x = a.x - b.x;
    long double y = a.y - b.y;
    long double z = a.z - b.z;
    return point(x, y, z);
}

long double dot_product (struct point a, struct point b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

void insert_face (int a, int b, int c) {
    struct point normal = cross_product(subtract(points[b], points[a]), subtract(points[c], points[a]));
    convex_hull.push_back(face(a, b, c, normal));
    in_convex_hull[a][b] = true;
    in_convex_hull[b][c] = true;
    in_convex_hull[c][a] = true;
}

void remove_face (struct face f) {
    in_convex_hull[f.a][f.b] = false;
    in_convex_hull[f.b][f.c] = false;
    in_convex_hull[f.c][f.a] = false;
}

bool is_visible (struct point p, struct face f) {
    if (dot_product(subtract(p, points[f.a]), f.normal) > epsilon)
        return true;
    else
        return false;
}

void Initialise_Hull () {
    insert_face(0, 1, 2);
    insert_face(0, 2, 1);
}

void Initialise (){
    cin >> no_of_points;

    points.resize(no_of_points);
    in_convex_hull.resize(no_of_points, vector<bool>(no_of_points, false));

    for (int i = 0; i < no_of_points; i++) {
        cin >> points[i].x >> points[i].y >> points[i].z;
    }
}

void Insert_Point (int i) {
    vector<face> new_convex_hull;

    for (int j = 0; j < convex_hull.size(); j++) {
        if (is_visible(points[i], convex_hull[j]))
            remove_face(convex_hull[j]);
        else
            new_convex_hull.push_back(convex_hull[j]);
    }

    convex_hull = new_convex_hull;
    for (int j = 0; j < convex_hull.size(); j++) {
        int arr[3] = {convex_hull[j].a, convex_hull[j].b, convex_hull[j].c};
        for (int k = 0; k < 3; k++) {
            if (!in_convex_hull[arr[(k + 1) % 3]][arr[k]])
                insert_face(arr[(k + 1) % 3], arr[k], i);
        }
    }
}

int main(){
    Initialise();
    if (no_of_points >= 3) Initialise_Hull();
    for (int i = 3; i < no_of_points; i++){
        Insert_Point(i);
    }

    // To get output on terminal: (Prints indices of points)
    // cout << convex_hull.size() << "\n";
    // for (int i = 0; i < convex_hull.size(); i++) {
    //     cout << convex_hull[i].a << " " << convex_hull[i].b << " " << convex_hull[i].c << "\n";
    // }


    //STL Format Output:
    ofstream fout;
    fout.open("hull.stl");
    fout << "solid convex_hull" << "\n";
    for (int i = 0; i < convex_hull.size(); i++) {
        fout << "facet normal " << convex_hull[i].normal.x << " " << convex_hull[i].normal.y << " " << convex_hull[i].normal.z << "\n";
        fout << "\touter loop" << "\n";
        fout << "\t\tvertex " << points[convex_hull[i].a].x << " " << points[convex_hull[i].a].y << " " << points[convex_hull[i].a].z << "\n";
        fout << "\t\tvertex " << points[convex_hull[i].b].x << " " << points[convex_hull[i].b].y << " " << points[convex_hull[i].b].z << "\n";
        fout << "\t\tvertex " << points[convex_hull[i].c].x << " " << points[convex_hull[i].c].y << " " << points[convex_hull[i].c].z << "\n";
        fout << "\tendloop" << "\n";
        fout << "endfacet" << "\n";
    }
    fout << "endsolid convex_hull" << "\n";
    fout.close();
}
