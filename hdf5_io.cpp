#include "hdf5_io.h"

hdf5_io::hdf5_io()
{

}

void hdf5_io::openfile()
{
    QString filename = datafile_name.split('.').at(0) + ".h5";
    HighFive::File this_file(filename.toStdString(), HighFive::File::ReadWrite | HighFive::File::Create | HighFive::File::Truncate);
}


void hdf5_io::hdf5_test_solve(int time_step)
{
    MatrixXd v = MatrixXd::Random(nx, ny);
    std::vector<double> write_vector(v.data(), v.data() + v.rows() * v.cols());
    qDebug() << write_vector;
    qDebug() << "Step " << time_step << "calculated\n";
    save_cube_on_timestep(write_vector, "pressure_cube", time_step);
    save_cube_on_timestep(write_vector, "saturation_cube", time_step);
    qDebug() << "Step " << time_step << "saved\n";
}

void hdf5_io::get_cube_on_timestep(std::string cube_name, int time_step, std::vector<double> *cube)
{
    QString filename = datafile_name.split('.').at(0) + ".h5";
    HighFive::File this_file(filename.toStdString(), HighFive::File::ReadOnly);
    QString dataset_name = QString(cube_name.c_str()) + QString("/") + QString::number(time_step);
    HighFive::DataSet dataset = this_file.getDataSet(dataset_name.toStdString());
    dataset.read(*cube);
}

void hdf5_io::save_cube_on_timestep(std::vector<double> &cube, std::string cube_name, int time_step)
{
    try
    {
        HighFive::Group this_group;
        QString filename = datafile_name.split('.').at(0) + ".h5";
        HighFive::File this_file(filename.toStdString(), HighFive::File::ReadWrite);
        this_group = (this_file.exist(cube_name)) ? this_file.getGroup(cube_name) : this_file.createGroup(cube_name);
        std::vector<int> v_dims(2);
        v_dims[0] = nx;
        v_dims[1] = ny;
        size_t dims = cube.size();
        HighFive::DataSet dataset = this_group.createDataSet<double>(std::to_string(time_step), HighFive::DataSpace(dims));
        HighFive::Attribute a_dims = dataset.createAttribute<int>("dimensions", HighFive::DataSpace::From(v_dims));
        a_dims.write(v_dims);
        dataset.write(cube.data());
    }
    catch(HighFive::Exception& err)
    {
        std::cerr << err.what() << std::endl;
    } 
}
