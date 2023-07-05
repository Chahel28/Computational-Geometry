#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <GL/glut.h>

using namespace std;

vector<pair<float,float>> vertices;
vector<vector<int>> triangles;

void displayTriangulatedPolygon(void){
    glClear(GL_COLOR_BUFFER_BIT);
    for (auto t: triangles){
        for (int i=0; i<3; i++){
            int a = t[i], b = t[(i+1)%3];
            glBegin(GL_LINES);
                glVertex3f((float)vertices[a].first/1000.0, (float)vertices[a].second/750.0, 0.0);
                glVertex3f((float)vertices[b].first/1000.0, (float)vertices[b].second/750.0, 0.0);
            glEnd();
        }
    }
    glFlush();
}

int main(int argc, char** argv){
    int n;
    cin >> n;
    vertices.resize(n);
    for (int i = 0; i < n; i++){
        cin >> vertices[i].first >> vertices[i].second;
    }

    list<int> polygon, ears, reflex_vertices, convex_vertices;
    for (int i=0; i<n; i++) polygon.push_back(i);

    //find reflex and convex vertices
    for (auto it=polygon.begin(); it!=polygon.end(); it++){
        int curr = *it;
        int prev = (curr+polygon.size()-1)%polygon.size();
        int next = (curr+1)%polygon.size();

        float x1 = vertices[prev].first;
        float y1 = vertices[prev].second;
        float x2 = vertices[curr].first;
        float y2 = vertices[curr].second;
        float x3 = vertices[next].first;
        float y3 = vertices[next].second;

        float cross_product = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);

        if (cross_product > (float)0.00001) convex_vertices.push_back(curr);
        else reflex_vertices.push_back(curr);
    }

    //find ears
    for (auto it=convex_vertices.begin(); it!=convex_vertices.end(); it++){
        int cur = *it;
        int prev = (cur+polygon.size()-1)%polygon.size();
        int next = (cur+1)%polygon.size();

        bool is_ear = true;
        if (reflex_vertices.size()!=0) {
            for (auto it2=reflex_vertices.begin(); it2!=reflex_vertices.end(); it2++){
                if (*it2==prev || *it2==cur || *it2==next) continue;
                float x1 = vertices[prev].first;
                float y1 = vertices[prev].second;
                float x2 = vertices[cur].first;
                float y2 = vertices[cur].second;
                float x3 = vertices[next].first;
                float y3 = vertices[next].second;
                float x4 = vertices[*it2].first;
                float y4 = vertices[*it2].second;

                float cross_product1 = (x2-x1)*(y4-y1) - (y2-y1)*(x4-x1);
                float cross_product2 = (x3-x2)*(y4-y2) - (y3-y2)*(x4-x2);
                float cross_product3 = (x1-x3)*(y4-y3) - (y1-y3)*(x4-x3);

                if (cross_product1 > 0.00001 && cross_product2 > 0.00001 && cross_product3 > 0.00001){
                    is_ear = false;
                    break;
                }
            }
        }
        if (is_ear) ears.push_back(cur);
    }

    //triangulate
    while (polygon.size() >= 3){
        int ear = ears.front();
        ears.pop_front();
        auto ear_it = find(polygon.begin(), polygon.end(), ear);
        int prev;
        if (ear_it == polygon.begin()){
            auto temp_it = polygon.end();
            temp_it--;
            prev = *temp_it;
        }
        else {
            auto temp_it = ear_it;
            temp_it--;
            prev = *temp_it;
        }
        int next;
        auto temp_it = ear_it;
        temp_it++;
        if (temp_it == polygon.end()) next = *polygon.begin();
        else next = *temp_it;
        vector<int> triangle = {prev, ear, next};
        triangles.push_back(triangle);
        polygon.remove(ear);
        convex_vertices.remove(ear);
        ears.remove(ear);
        ears.remove(prev);
        ears.remove(next);
        reflex_vertices.remove(prev);
        reflex_vertices.remove(next);
        convex_vertices.remove(prev);
        convex_vertices.remove(next);

        // check if prev and next are convex
        auto prev_it = find(polygon.begin(), polygon.end(), prev);
        auto next_it = find(polygon.begin(), polygon.end(), next);
        int prev_prev;
        if (prev_it==polygon.begin()) {
            auto temp_it = polygon.end();
            temp_it--;
            prev_prev = *temp_it;
        }
        else {
            auto temp_it = prev_it;
            temp_it--;
            prev_prev = *temp_it;
        }
        int next_next;
        temp_it = next_it;
        temp_it++;
        if (temp_it==polygon.end()) next_next = *polygon.begin();
        else next_next = *temp_it;

        float x1 = vertices[prev_prev].first;
        float y1 = vertices[prev_prev].second;
        float x2 = vertices[prev].first;
        float y2 = vertices[prev].second;
        float x3 = vertices[next].first;
        float y3 = vertices[next].second;

        float cross_product = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);

        bool prev_is_convex = false, next_is_convex = false;

        if (cross_product > 0.00001){
            convex_vertices.push_back(prev);
            prev_is_convex = true;
        }
        else{
            reflex_vertices.push_back(prev);
        }

        x1 = vertices[prev].first;
        y1 = vertices[prev].second;
        x2 = vertices[next].first;
        y2 = vertices[next].second;
        x3 = vertices[next_next].first;
        y3 = vertices[next_next].second;

        cross_product = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);

        if (cross_product > 0.00001){
            convex_vertices.push_back(next);
            next_is_convex = true;
        }
        else{
            reflex_vertices.push_back(next);
        }

        //check if prev and next are ears
        if (prev_is_convex){
            bool is_ear = true;
            for (auto it2=reflex_vertices.begin(); it2!=reflex_vertices.end(); it2++){
                if (*it2 == prev) continue;
                if (*it2 == next) continue;
                if (*it2 == prev_prev) continue;
                float x1 = vertices[prev_prev].first;
                float y1 = vertices[prev_prev].second;
                float x2 = vertices[prev].first;
                float y2 = vertices[prev].second;
                float x3 = vertices[next].first;
                float y3 = vertices[next].second;
                float x4 = vertices[*it2].first;
                float y4 = vertices[*it2].second;

                float cross_product1 = (x2-x1)*(y4-y1) - (y2-y1)*(x4-x1);
                float cross_product2 = (x3-x2)*(y4-y2) - (y3-y2)*(x4-x2);
                float cross_product3 = (x1-x3)*(y4-y3) - (y1-y3)*(x4-x3);

                if (cross_product1 > 0.00001 && cross_product2 > 0.00001 && cross_product3 > 0.00001){
                    is_ear = false;
                    break;
                }
            }
            if (is_ear) ears.push_back(prev);
        }
        if (next_is_convex){
            bool is_ear = true;
            for (auto it2=reflex_vertices.begin(); it2!=reflex_vertices.end(); it2++){
                if (*it2 == prev) continue;
                if (*it2 == next) continue;
                if (*it2 == next_next) continue;
                float x1 = vertices[prev].first;
                float y1 = vertices[prev].second;
                float x2 = vertices[next].first;
                float y2 = vertices[next].second;
                float x3 = vertices[next_next].first;
                float y3 = vertices[next_next].second;
                float x4 = vertices[*it2].first;
                float y4 = vertices[*it2].second;

                float cross_product1 = (x2-x1)*(y4-y1) - (y2-y1)*(x4-x1);
                float cross_product2 = (x3-x2)*(y4-y2) - (y3-y2)*(x4-x2);
                float cross_product3 = (x1-x3)*(y4-y3) - (y1-y3)*(x4-x3);

                if (cross_product1 > 0.00001 && cross_product2 > 0.00001 && cross_product3 > 0.00001){
                    is_ear = false;
                    break;
                }
            }
            if (is_ear) ears.push_back(next);
        }
    }

    
    cout << triangles.size() << endl;
    for (auto t: triangles){
        cout<<"{";
        for (int i=0; i<3; i++){
            cout << "(" << vertices[t[i]].first << "," << vertices[t[i]].second << ")";
            if (i<2) cout << ", ";
        }
        cout << "}" << endl;
    }

    //plot the triangles
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1000, 750);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Triangulated Polygon");
    glutDisplayFunc(displayTriangulatedPolygon);
    glutMainLoop();

    return 0;
}