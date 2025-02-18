import os
import argparse
import json
import requests
import sys
from mako.lookup import TemplateLookup

def write_template_dict_to_file(
    template_path,
    template_file,
    source_dict,
    output_file,
    template_out_directory,
    comp_template_directory,
):

    """
    Generate output file with new source to be substituted from template

    :param: template_path: abs path to templates
    :param: template_file: template file uri (relative to template_path)
    :param: source_dict: source dictionary for template processing
    :param: output_file: generated file name (abs path)
    :return: err string (None if success)
    """
    try:
        with open(output_file, "w", encoding="utf-8") as outfile:
            try:
                my_lookup = TemplateLookup(
                    directories=[template_path],
                    module_directory=TEMP_DIRECTORY_FOR_MAKO_MODULES.format(
                        template_out_directory, comp_template_directory
                    ),
                    strict_undefined=True,
                )
                my_template = my_lookup.get_template(template_file)
                outfile.write(my_template.render(source_dict))
            except TypeError as err:
                return "failed to render due to TypeError: {}".format(err)
            except IndexError as err:
                return "failed to render due to IndexError: {}".format(err)
            except KeyError as err:
                return "failed to render due to KeyError: {}".format(err)
            except FileNotFoundError as err:
                return "file not found: {}".format(err)
    except IOError as err:
        return "failed to create {}: {}".format(output_file, err)

    return None

def generate_msg_parser_files(output_dir, templates_input_dir, template_out_directory):



    # msg_parser.cpp
    output_file_name = MSG_PARSER_SRC_FILE_NAME
    os.makedirs(os.path.join(output_dir, "src"), exist_ok=True)
    output_file_path = "{}/src/{}".format(
        output_dir, output_file_name
    )
    cmake_files.append("/src/{}".format(output_file_name))
    err = write_template_dict_to_file(
        templates_input_dir,
        MSG_PARSER_SRC_TEMPLATE_FILE,
        source_dict,
        output_file_path,
        template_out_directory,
        MAKO_TEMP_OUTPUT_DIR,
    )
    if err is not None:
        return "failed to create {}, err info: {}".format(output_file_name, err)

    source_dict["files"] = cmake_files
    err = write_template_dict_to_file(
        templates_input_dir,
        CMAKE_TEMPLATE_FILE,
        source_dict,
        output_file_path,
        template_out_directory,
        MAKO_TEMP_OUTPUT_DIR,
    )
    if err is not None:
        return "failed to create {}, err info: {}".format(
            CMAKE_FILE_NAME, err
        )

def main():
    print("************************* generator Starting*********************************")

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--build-output",
        dest="build_output",
        required=True,
        type=str,
        help="The output directory of the project build",
    )

    ret = generate_msg_parser_files(output_dir, template_input_path, template_output_path)
    if ret != None:
        print(ret)
        sys.exit(-1)

    print("*************************sysm_msg_parser generator Finished*********************************")

if __name__ == "__main__":
    main()
