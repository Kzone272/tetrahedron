#include <cmath>
#include <iostream>
#include <ios>
#include <vector>

#include "Eigen/Core"
#include <Eigen/Geometry>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

struct Triangle {
  // Indices
  size_t ai;
  size_t bi;
  size_t ci;
};

struct Geometry {
  std::vector<Eigen::Vector3f> vertices;
  std::vector<Triangle> triangles;
};

Geometry tetrahedron() {
  Eigen::Vector3f a(0, 2. / 3., 0);
  Eigen::AngleAxisf r120 = Eigen::AngleAxisf(2. / 3 * M_PI, Eigen::Vector3f::UnitZ());
  Eigen::Vector3f b = r120 * a;
  Eigen::Vector3f c = r120 * b;

  float lift = sqrt(8. / 9.);
  Eigen::Vector3f d(0, 0, lift);

  Geometry tetra;
  tetra.vertices.insert(tetra.vertices.begin(), {a, b, c, d});
  size_t ai = 0, bi = 1, ci = 2, di = 3;

  tetra.triangles.push_back({ai, ci, bi});
  tetra.triangles.push_back({ai, bi, di});
  tetra.triangles.push_back({bi, ci, di});
  tetra.triangles.push_back({ci, ai, di});

  return std::move(tetra);
};

Eigen::IOFormat comma;

void iter(Geometry& geom, int steps) {
  for (int i = 0; i < steps; i++) {
    std::vector<Triangle> new_triangles;
    for (const auto& t : geom.triangles) {
      const Eigen::Vector3f a = geom.vertices[t.ai];
      const Eigen::Vector3f b = geom.vertices[t.bi];
      const Eigen::Vector3f c = geom.vertices[t.ci];

      Eigen::Vector3f d = (b + c) / 2.;
      size_t di = geom.vertices.size();
      geom.vertices.push_back(d);
      Eigen::Vector3f e = (a + c) / 2.;
      size_t ei = geom.vertices.size();
      geom.vertices.push_back(e);
      Eigen::Vector3f f = (a + b) / 2.;
      size_t fi = geom.vertices.size();
      geom.vertices.push_back(f);

      size_t ai = t.ai;
      size_t bi = t.bi;
      size_t ci = t.ci;
      new_triangles.push_back({ai, fi, ei});
      new_triangles.push_back({fi, bi, di});
      new_triangles.push_back({di, ci, ei});

      // point on abc plane:
      Eigen::Vector3f gprime = (a + b + c) / 3.;
      Eigen::Vector3f ab = b - a;
      Eigen::Vector3f ac = c - a;
      Eigen::Vector3f normal = ab.cross(ac);
      normal.normalize();

      float lift = sqrt(8. / 9.) / 3.;
      // float lift = 1. / 4.;
      float height = (d - a).norm() * lift;
      Eigen::Vector3f g = gprime + height * normal;
      size_t gi = geom.vertices.size();
      geom.vertices.push_back(g);

      new_triangles.push_back({ei, fi, gi});
      new_triangles.push_back({fi, di, gi});
      new_triangles.push_back({di, ei, gi});
    }
    geom.triangles = std::move(new_triangles);
  }
}

int main() {
  comma = Eigen::IOFormat(Eigen::FullPrecision, Eigen::DontAlignCols, "", ",", "", "", "", "");

  Geometry tetra = tetrahedron();

  const int steps = 2;
  iter(tetra, steps);

  // The string formatting below was based on the Three.js library, which allowed
  // me to paste the code into a jsfiddle I found to render the results, without
  // setting up a whole rendering stack in C++.
  // JS Fiddle copy with my geometry: https://jsfiddle.net/qof68bmk/show
  //
  // The first section is the vertex-array, the second section is an index-array of
  // triangles based on the points in the vertex-array.

  for (auto& point : tetra.vertices) {
    std::cout << std::fixed
              <<"new THREE.Vector3(" <<  point.format(comma) << "),\n";
  }

  std::cout << "\n\n";

  for (auto &t : tetra.triangles)
  {
    std::cout << "new THREE.Face3(" << t.ai
              << ", " << t.bi
              << ", " << t.ci
              << "),\n";
  }
  // draw(triangles);
  return 0;
}
