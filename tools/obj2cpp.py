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
        h_file.write("// Auto-generated file with definition of a {} mesh\n".format(mesh_name))
        h_file.write("// Number of triangles: {}\n".format(len(face_lines)))
        h_file.write("// Number of vertices: {}\n".format(len(vertice_lines)))
        h_file.write("// Number of normals: {}\n".format(len(normal_lines)))
        h_file.write("// Number of texture mappings: {}\n".format(len(uv_lines)))
        h_file.write("\n")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
        h_file.write("")
    with open(os.path.join(output_folder, mesh_name + ".c"), "w+") as c_file:
        pass


if __name__ == "__main__":
    main(sys.argv[1:])