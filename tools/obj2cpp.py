import os
import sys
import argparse


def main(argv):
    parser = argparse.ArgumentParser(
        description="Convert a Wavefront OBJ file to C code. "
                    "The input file is expected to contain vertices, normals, "
                    "vertices uv mappings and faces. "
                    "The faces must be triangulated."
    )
    parser.add_argument(
        "input-file",
        help="The input .obj file"
    )
    parser.add_argument(
        "output_folder",
        required=False,
        help="The folder to store the converted files in. "
             "Defaults to the input file's folder."
    )

    args = parser.parse_args(argv)
    vertice_lines = list()
    normal_lines = list()
    uv_lines = list()
    face_lines = list()
    with open(args.input_file) as obj_file:
        for line in obj_file.readlines():
            if line.startswith("v "):
                vertice_lines.append(line[2: ]).strip()
            elif line.startswith("vt "):
                uv_lines.append(line[3: ]).strip()
            elif line.startswith("vn "):
                normal_lines.append(line[3: ]).strip()
            elif line.startswith("f "):
                face_lines.append(line[2: ]).strip()
    output_folder = args.output_folder or os.path.dirname(args.input_file)
    mesh_name = os.path.splitext(os.path.basename(args.input_file))[0]
    with open(os.path.join(output_folder, mesh_name + ".h"), "w+") as h_file:
        position_count = len(vertice_lines)
        normal_count = len(normal_lines)
        uv_mapping_count = len(uv_lines)
        triangle_count = len(face_lines)
        vertice_count = len(face_lines) * 3

        h_file.write("// Auto-generated file with definition of a {} mesh\n".format(mesh_name))
        h_file.write("// Positions: {}\n".format(position_count))
        h_file.write("// Normals: {}\n".format(normal_count))
        h_file.write("// Texture mappings: {}\n".format(uv_mapping_count))
        h_file.write("// Triangles: {}\n".format(triangle_count)))
        h_file.write("// Vertices: {}\n".format(vertice_count))
        h_file.write("\n")
        h_file.write("const int vertice_count;\n")
        h_file.write("const float positions[{}];\n".format(position_count))
        h_file.write("const float texture_mapping[{}];\n".format(uv_mapping_count)))
        h_file.write("const float normals[{}];\n".format(normal_count))
    with open(os.path.join(output_folder, mesh_name + ".c"), "w+") as c_file:
        c_file.write("// Auto-generated file with definition of a {} mesh\n".format(mesh_name))
        c_file.write('#include "{}.h"\n'.format(mesh_name))
        c_file.write("\n")
        c_file.write("const int vertice_count = {}\n".format(vertice_count))
        c_file.write("const float cubePositions[{}] = \n".format(vertice_count * 3))
        c_file.write("{\n")
        for i in range(triangle_count):
            pass
        c_file.write("\n")
        c_file.write("\n")
        c_file.write("\n")
        c_file.write("\n")
        c_file.write("\n")


if __name__ == "__main__":
    main(sys.argv[1:])