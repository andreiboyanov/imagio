import os
import sys
import argparse


def parse_2d_line(line):
    x, y = line.strip().split(" ")
    return float(x), float(y)


def parse_3d_line(line):
    x, y, z = line.strip().split(" ")
    return float(x), float(y), float(z)


def parse_triangle_line(line, positions, normals, texture_coordinates):
    triangle_vertices = list()
    triangle_normals  = list()
    triangle_textures = list()
    for triangle_point in line.strip().split(" "):
        vertex_index, texture_index, normal_index = triangle_point.split("/")
        vertex_index = int(vertex_index) - 1
        triangle_vertices.append(positions[vertex_index])
        normal_index = int(normal_index) - 1
        triangle_normals.append(normals[normal_index])
        texture_index = int(texture_index) - 1
        triangle_textures.append(texture_coordinates[texture_index])
    return triangle_vertices, triangle_normals, triangle_textures


def main(argv):
    parser = argparse.ArgumentParser(
        description="Convert a Wavefront OBJ file to C code. "
                    "The input file is expected to contain vertices, normals, "
                    "vertices uv mappings and faces. "
                    "The faces must be triangulated."
    )
    parser.add_argument(
        "input_file",
        help="The input .obj file"
    )
    parser.add_argument(
        "-o",
        "--output-folder",
        help="The folder to store the converted files in. "
             "Defaults to the input file's folder."
    )

    args = parser.parse_args(argv)
    positions = list()
    normals = list()
    texture_mapping = list()
    faces = dict(
        vertices = list(),
        normals = list(),
        texture_mapping = list()
    )
    with open(args.input_file) as obj_file:
        for line in obj_file.readlines():
            if line.startswith("v "):
                position = parse_3d_line(line[2: ])
                positions.append(position)
            elif line.startswith("vt "):
                texture_coordinates = parse_2d_line(line[3: ])
                texture_mapping.append(texture_coordinates)
            elif line.startswith("vn "):
                normal = parse_3d_line(line[3: ])
                normals.append(normal)
            elif line.startswith("f "):
                triangle_vertices, triangle_normals, triangle_texture_coordinates = parse_triangle_line(
                    line[2:], positions, normals, texture_mapping
                )
                faces["vertices"].append(triangle_vertices)
                faces["normals"].append(triangle_normals)
                faces["texture_mapping"].append(triangle_texture_coordinates)

    output_folder = args.output_folder or os.path.dirname(args.input_file)
    mesh_name = os.path.splitext(os.path.basename(args.input_file))[0]

    position_count = len(positions)
    normal_count = len(normals)
    uv_mapping_count = len(texture_mapping)
    triangle_count = len(faces["vertices"])
    vertice_count = triangle_count * 3

    with open(os.path.join(output_folder, mesh_name + ".h"), "w+") as h_file:
        h_file.write("// Auto-generated file with definition of a {} mesh\n".format(mesh_name))
        h_file.write("// Positions: {}\n".format(position_count))
        h_file.write("// Normals: {}\n".format(normal_count))
        h_file.write("// Texture mappings: {}\n".format(uv_mapping_count))
        h_file.write("// Triangles: {}\n".format(triangle_count))
        h_file.write("// Vertices: {}\n".format(vertice_count))
        h_file.write("\n")
        h_file.write("namespace imagio\n")
        h_file.write("{\n")
        h_file.write("namespace meshes\n")
        h_file.write("{\n")
        h_file.write("namespace {}\n".format(mesh_name))
        h_file.write("{\n")
        h_file.write("const int vertice_count = {};\n".format(vertice_count))
        h_file.write("const float vertices[{}] = \n".format(vertice_count * 3))
        h_file.write("{\n")
        for i in range(triangle_count):
            vertex = faces["vertices"][i]
            h_file.write("\t{}, {}, {},\n".format(vertex[0][0], vertex[0][1], vertex[0][2]))
            h_file.write("\t{}, {}, {},\n".format(vertex[1][0], vertex[1][1], vertex[1][2]))
            h_file.write("\t{}, {}, {},\n".format(vertex[2][0], vertex[2][1], vertex[2][2]))
        h_file.write("};\n")
        h_file.write("const float normals[{}] = \n".format(vertice_count * 3))
        h_file.write("{\n")
        for i in range(triangle_count):
            normal = faces["normals"][i]
            h_file.write("\t{}, {}, {},\n".format(normal[0][0], normal[0][1], normal[0][2]))
            h_file.write("\t{}, {}, {},\n".format(normal[1][0], normal[1][1], normal[1][2]))
            h_file.write("\t{}, {}, {},\n".format(normal[2][0], normal[2][1], normal[2][2]))
        h_file.write("};\n")
        h_file.write("const float texture_coordinates[{}] = \n".format(vertice_count * 2))
        h_file.write("{\n")
        for i in range(triangle_count):
            uv = faces["texture_mapping"][i]
            h_file.write("\t{}, {},\n".format(uv[0][0], uv[0][1]))
            h_file.write("\t{}, {},\n".format(uv[1][0], uv[1][1]))
        h_file.write("};\n")
        h_file.write("};\n")
        h_file.write("};\n")
        h_file.write("};\n")


if __name__ == "__main__":
    main(sys.argv[1:])