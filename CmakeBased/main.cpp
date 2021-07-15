#include <iostream>
#include <opencv2/opencv.hpp>

#include "main_func.cpp"

#include "config.h"
#include "viewmodel.hpp"
extern ViewModel vm;

int main(int argc, const char** argv)
{
    std::vector<Triangle*> TriangleList;

    float angle = 140.0;
    bool command_line = false;

    std::string filename = "output.png";
    objl::Loader Loader;
    std::string cur_path(CUR_SRC_DIR);
    std::string obj_path = cur_path + "/models/spot/";

    // Load .obj File
    bool loadout = Loader.LoadFile(cur_path + "/models/spot/spot_triangulated_good.obj");
    for (auto mesh : Loader.LoadedMeshes)
    {
        for (int i = 0; i < mesh.Vertices.size(); i += 3)
        {
            Triangle* t = new Triangle();
            for (int j = 0; j < 3; j++)
            {
                t->setVertex(j, Vector4f(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z, 1.0));
                t->setNormal(j, Vector3f(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
                t->setTexCoord(j, Vector2f(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
            }
            TriangleList.push_back(t);
        }
    }

    rst::rasterizer r(700, 700);

    auto texture_path = "hmap.jpg";
    r.set_texture(Texture(obj_path + texture_path));

    std::function<Eigen::Vector3f(fragment_shader_payload)> active_shader = phong_fragment_shader;

//    if (argc >= 2)
//    {
//        command_line = true;
//        filename = std::string(argv[1]);
//
//        if (argc == 3 && std::string(argv[2]) == "texture")
//        {
//            std::cout << "Rasterizing using the texture shader\n";
//            active_shader = texture_fragment_shader;
//            texture_path = "spot_texture.png";
//            r.set_texture(Texture(obj_path + texture_path));
//        }
//        else if (argc == 3 && std::string(argv[2]) == "normal")
//        {
//            std::cout << "Rasterizing using the normal shader\n";
//            active_shader = normal_fragment_shader;
//        }
//        else if (argc == 3 && std::string(argv[2]) == "phong")
//        {
//            std::cout << "Rasterizing using the phong shader\n";
//            active_shader = phong_fragment_shader;
//        }
//        else if (argc == 3 && std::string(argv[2]) == "bump")
//        {
//            std::cout << "Rasterizing using the bump shader\n";
//            active_shader = bump_fragment_shader;
//        }
//        else if (argc == 3 && std::string(argv[2]) == "displacement")
//        {
//            std::cout << "Rasterizing using the bump shader\n";
//            active_shader = displacement_fragment_shader;
//        }
//    }

    Eigen::Vector3f eye_pos = { 0,0,10 };

    r.set_vertex_shader(vertex_shader);
    r.set_fragment_shader(active_shader);

    int key = 0;
    int frame_count = 0;

//    if (command_line)
//    {
//        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
//        r.set_model(get_model_matrix(angle));
//        r.set_view(get_view_matrix(eye_pos));
//        r.set_projection(get_projection_matrix(45.0, 1, 0.1, 50));
//
//        r.draw(TriangleList);
//        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
//        image.convertTo(image, CV_8UC3, 1.0f);
//        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
//
//        cv::imwrite(filename, image);
//
//        return 0;
//    }

    vm=ViewModel(r,eye_pos,TriangleList,filename,angle,45.0, 1, 0.1, 50);
    vm.viewInit();

    return 0;
}