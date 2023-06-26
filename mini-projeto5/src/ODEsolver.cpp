#include "ODEsolver.h"

using namespace std;

/*
dy/dt = v = f[0](t,y,v)
dv/dt = -g = f[1](t,y,v)

ODEpoint: [t, (y,v)]

ODEsolver: euler, ...

*/

const vector<ODEpoint>& ODEsolver::Euler(ODEpoint i, double step, double T){

    vector<ODEpoint> V1;
    V1.push_back(i);

    function<double(double)> dtheta_dt = F[0];
    function<double(double)> domega_dt = F[1];

    ODEpoint Pcur = V1.back();

    double theta = Pcur.X()[0];
    double omega = Pcur.X()[1];

    while (V1.back().T() < T-step) {
        Pcur = V1.back();

        // Atualizar as variáveis usando o método de Euler
        double omega_next = omega + domega_dt(theta) * step;
        double theta_next = theta + omega * step;

        // Atualizar as variáveis
        theta = theta_next;
        omega = omega_next;

        ODEpoint Pnext(Pcur.T()+step, {theta, omega} );
        V1.push_back(Pnext);
    }

    FullKey.push_back("Euler");
    MS["Euler"] = V1;
    return MS["Euler"];

}

const vector<ODEpoint>& ODEsolver::PredictorCorrector(ODEpoint i, double step, double T){

    vector<ODEpoint> V1;
    V1.push_back(i);

    function<double(double)> dtheta_dt = F[0];
    function<double(double)> domega_dt = F[1];

    ODEpoint Pcur = V1.back();

    double theta = Pcur.X()[0];
    double omega = Pcur.X()[1];

    while (V1.back().T() < T-step) {
        Pcur = V1.back();

        // Passo de predição usando o método de Euler
        double theta_pred = theta + step * dtheta_dt(omega);
        double omega_pred = omega + step * domega_dt(theta);

        // Passo de correção usando o método de Euler modificado
        double theta_corr = theta + 0.5 * step * (dtheta_dt(omega) + dtheta_dt(omega_pred));
        double omega_corr = omega + 0.5 * step * (domega_dt(theta) + domega_dt(theta_pred));

        // Atualizar as variáveis
        theta = theta_corr;
        omega = omega_corr;
        ODEpoint Pnext(Pcur.T()+step, {theta, omega} );
        V1.push_back(Pnext);
    }

    FullKey.push_back("PredictorCorrector");
    MS["PredictorCorrector"] = V1;
    return MS["PredictorCorrector"];
}

const vector<ODEpoint>& ODEsolver::LeapFrog(ODEpoint i, double step, double T){

    vector<ODEpoint> V1;
    V1.push_back(i);

    function<double(double)> dtheta_dt = F[0];
    function<double(double)> domega_dt = F[1];

    ODEpoint Pcur = V1.back();

    double theta = Pcur.X()[0];
    double omega = Pcur.X()[1];

    // Primeira iteração do método de Leapfrog
    double theta_half = theta + 0.5 * step * omega;

    while (V1.back().T() < T-step)  {
        Pcur = V1.back();

        // Calcular as próximas aproximações usando o método de Leapfrog
        double omega_next = omega + domega_dt(theta_half) * step;
        double theta_next = theta + omega_next * step;

        // Atualizar as variáveis
        theta = theta_next;
        omega = omega_next;

        // Atualizar theta_half para a próxima iteração
        theta_half = theta + 0.5 * step * omega;

        // Adicionar o ponto à solução
        ODEpoint Pnext(Pcur.T() + step, {theta, omega});
        V1.push_back(Pnext);
    }

    FullKey.push_back("LeapFrog");
    MS["LeapFrog"] = V1;
    return MS["LeapFrog"];

}

const vector<ODEpoint>& ODEsolver::RK2(ODEpoint i, double step, double T) {

    vector<ODEpoint> V1;
    V1.push_back(i);

    function<double(double)> dtheta_dt = F[0];
    function<double(double)> domega_dt = F[1];

    ODEpoint Pcur = V1.back();

    double theta = Pcur.X()[0];
    double omega = Pcur.X()[1];

    while (V1.back().T() < T-step) {
        Pcur = V1.back();

        // Primeiro passo do método de Runge-Kutta 2K
        double k1_theta = step * dtheta_dt(omega);
        double k1_omega = step * domega_dt(theta);

        // Segundo passo do método de Runge-Kutta 2K
        double k2_theta = step * dtheta_dt(omega + 0.5 * k1_omega);
        double k2_omega = step * domega_dt(theta + 0.5 * k1_theta);

        // Atualizar as variáveis
        theta = theta + k2_theta;
        omega = omega + k2_omega;
        ODEpoint Pnext(Pcur.T()+step, {theta, omega} );
        V1.push_back(Pnext);
    }

    FullKey.push_back("RK2");
    MS["RK2"] = V1;
    return MS["RK2"];
}

const vector<ODEpoint>& ODEsolver::RK4(ODEpoint i, double step, double T){

    vector<ODEpoint> V1;
    V1.push_back(i);

    function<double(double)> dtheta_dt = F[0];
    function<double(double)> domega_dt = F[1];

    ODEpoint Pcur = V1.back();

    double theta = Pcur.X()[0];
    double omega = Pcur.X()[1];


    while (V1.back().T() < T-step) {
        Pcur = V1.back();

        // Primeiro passo do método de Runge-Kutta 4K
        double k1_theta = step * dtheta_dt(omega);
        double k1_omega = step * domega_dt(theta);

        // Segundo passo do método de Runge-Kutta 4K
        double k2_theta = step * dtheta_dt(omega + 0.5 * k1_omega);
        double k2_omega = step * domega_dt(theta + 0.5 * k1_theta);

        // Terceiro passo do método de Runge-Kutta 4K
        double k3_theta = step * dtheta_dt(omega + 0.5 * k2_omega);
        double k3_omega = step * domega_dt(theta + 0.5 * k2_theta);

        // Quarto passo do método de Runge-Kutta 4K
        double k4_theta = step * dtheta_dt(omega + k3_omega);
        double k4_omega = step * domega_dt(theta + k3_theta);

        // Atualizar as variáveis
        theta = theta + (1.0 / 6.0) * (k1_theta + 2.0 * k2_theta + 2.0 * k3_theta + k4_theta);
        omega = omega + (1.0 / 6.0) * (k1_omega + 2.0 * k2_omega + 2.0 * k3_omega + k4_omega);
        ODEpoint Pnext(Pcur.T()+step, {theta, omega} );
        V1.push_back(Pnext);
    }

    FullKey.push_back("RK4");
    MS["RK4"] = V1;
    return MS["RK4"];
}

const void ODEsolver::printMS(string key){

    if (key == ""){
        for (int a = 0; a<FullKey.size(); a++){
            printMS(FullKey[a]);
        }
    }
    else{
        cout<<endl;
        cout<<key<<":"<<endl;
        cout<<"{ time, theta, omega }"<<endl;
        for (int a = 0; a<MS[key].size(); a++){
            cout<<"{ ";
            cout<<MS[key][a].T()<<", "<<MS[key][a].X()[0]<<", "<<MS[key][a].X()[1];
            cout<<" }"<<endl;
        }

        for (int i = 0; i<MS[key][0].X().size(); i++){
            string ystring = "y";
            if (i == 0){
               ystring = "theta";
            }
            else{
                ystring = "omega";
            }
            DrawGraphicst_x(MS[key], i, "t[s]", ystring, key);
        }

        DrawGraphicsx1_x2(MS[key], 0, 1, "theta", "omega", key);

    }

}

map<string, vector<ODEpoint> > ODEsolver::GetMS(){
    return MS;
}

void ODEsolver::DrawGraphicst_x(vector <ODEpoint> data, int variable = 0, string xtitle = "x", string ytitle = "y", string maintitle = "Graphics") {

    vector<double> xdata(data.size()), ydata(data.size());

    for (int a = 0; a<data.size(); a++){
        xdata[a] = data[a].T();
        ydata[a] = data[a].X()[variable];
    }

    TGraph *g = new TGraph(xdata.size(), xdata.data(), ydata.data());
    g->GetXaxis()->SetTitle(xtitle.c_str());
    g->GetYaxis()->SetTitle(ytitle.c_str());
    g->SetLineColor(kBlue - 2);
    g->SetLineWidth(4);
    g->SetTitle(maintitle.c_str());

    // display
    TCanvas* c = new TCanvas(("c_" + maintitle).c_str(), "Function Plot", 800, 600);
    g->Draw("ALP");
    c->Update();
    gSystem->ProcessEvents();
    c->WaitPrimitive();
    c->SaveAs(("Function_Plot_" + maintitle + "_" + xtitle + "_" + ytitle + ".pdf").c_str());

    delete g;
    delete c;
}

void ODEsolver::DrawGraphicsx1_x2(vector <ODEpoint> data, int variableX = 0, int variableY = 1, string xtitle = "x", string ytitle = "y", string maintitle = "Graphics"){

    vector<double> xdata(data.size()), ydata(data.size());

    for (int a = 0; a<data.size(); a++){
        xdata[a] = data[a].X()[variableX];
        ydata[a] = data[a].X()[variableY];
    }

    TGraph *g = new TGraph(xdata.size(), xdata.data(), ydata.data());
    g->GetXaxis()->SetTitle(xtitle.c_str());
    g->GetYaxis()->SetTitle(ytitle.c_str());
    g->SetLineColor(kBlue - 2);
    g->SetLineWidth(4);
    g->SetTitle(maintitle.c_str());

    // display
    TCanvas* c = new TCanvas(("c_" + maintitle).c_str(), "Function Plot", 600, 600);
    g->Draw("ALP");
    c->Update();
    gSystem->ProcessEvents();
    c->WaitPrimitive();
    c->SaveAs(("Function_Plot_" + maintitle + "_" + xtitle + "_" + ytitle + ".pdf").c_str());

    delete g;
    delete c;
}