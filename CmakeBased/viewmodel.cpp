
#include "viewmodel.h"
#include "Message.h"
#include "model.cpp"
#include <ctime>

extern Message msg_ctrl;

void ViewModel::vmInit() {
    model_main();
    redraw();

    msg_ctrl.registerPassF([this](){return this->getImage();});
    msg_ctrl.registerFunc("anglePlus",
                          [this](){this->modifyAngle(1);}
                          );
    msg_ctrl.registerFunc("angleMinus",
                          [this](){this->modifyAngle(-1);}
    );
    msg_ctrl.registerFunc("eyeLeft",
                          [this](){this->modifyEyepos(0.1,0);}
    );
    msg_ctrl.registerFunc("eyeRight",
                          [this](){this->modifyEyepos(-0.1,0);}
    );
    msg_ctrl.registerFunc("eyeUp",
                          [this](){this->modifyEyepos(0.1,1);}
    );
    msg_ctrl.registerFunc("eyeDown",
                          [this](){this->modifyEyepos(-0.1,1);}
    );
    msg_ctrl.registerFunc("eyeZPlus",
                          [this](){this->modifyEyepos(0.2,2);}
    );
    msg_ctrl.registerFunc("eyeZMinus",
                          [this](){this->modifyEyepos(-0.2,2);}
    );
    msg_ctrl.registerFunc("redraw",
                          [this](){this->redraw();}
    );

};

cv::Mat ViewModel::getImage(bool write) {
    cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
    image.convertTo(image, CV_8UC3, 1.0f);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    if (write) {
        cv::imwrite(filename, image);
    }
    return image;
}

void ViewModel::redraw() {
    cout << "rendering" << endl;
    r.clear(rst::Buffers::Color | rst::Buffers::Depth);
    r.set_model(get_model_matrix(angle));
    r.set_view(get_view_matrix(eye_pos));
    r.set_projection(get_projection_matrix(eye_fov,aspect_ratio,zNear,zFar));
    auto start_t=clock();
    r.draw(TriangleList);
    cout << '\r' << clock()-start_t << " ms" << ends;
    cout << "render completed" << endl;
}

void ViewModel::model_main() {

    bool command_line = false;

    std::string obj_path = cur_path + "/models/spot/";

    // Load .obj File
    objl::Loader Loader;
    bool loadout = Loader.LoadFile(cur_path + "/models/spot/spot_triangulated_good.obj");
    for (auto mesh : Loader.LoadedMeshes)
    {
        for (int i = 0; i < mesh.Vertices.size(); i += 3)
        {
            TriangleList.emplace_back(new Triangle());
            Triangle &t = **(TriangleList.rbegin());
            for (int j = 0; j < 3; j++)
            {
                t.setVertex(j, Vector4f(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z, 1.0));
                t.setNormal(j, Vector3f(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
                t.setTexCoord(j, Vector2f(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
            }
        }
    }


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


    r.set_vertex_shader(vertex_shader);
    r.set_fragment_shader(active_shader);

//    int frame_count = 0;

    if (command_line)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45.0, 1, 0.1, 50));

        r.draw(TriangleList);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

        cv::imwrite(filename, image);

        return ;
    }
}