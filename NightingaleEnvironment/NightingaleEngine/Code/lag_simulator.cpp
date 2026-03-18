#include "pch.h"
#include "lag_simulator.h"
#include <thread>

void LagSimulatorPanel::render_update()
{

    bool bRemainOpen{ true };

    ImGui::Begin("Lag Simulator", &bRemainOpen);

    ImGui::InputFloat("Lag Time (Seconds)", &m_lagTimeSeconds, 0.1f);

    if (ImGui::Button("Simulate Lag")) {
        long sleepTimeMilli = (long)(m_lagTimeSeconds * 1000.0f);
        
        if (sleepTimeMilli > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMilli));
        }
    }

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}
