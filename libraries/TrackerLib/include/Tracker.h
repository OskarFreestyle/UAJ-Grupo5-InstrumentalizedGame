#pragma once

class Tracker
{
private:
    Tracker();
    ~Tracker();

    // Variables
    // Puntero a la instancia estatica del singleton
    static Tracker* _instance;

    // TODO add persistenceObjects IPersistence
    // TODO add activeTrackers List<ITrackerAsset>

public:
    /// <summary>
    /// Inicializa el tracker
    /// </summary>
    void Init();

    /// <summary>
    /// Libera los recursos del tracker, persistiendo antes todos los eventos de la cola
    /// </summary>
    void End();

    /// <summary>
    /// Devuelve la instancia del singleton
    /// </summary>
    static Tracker& GetInstance();

    /// <summary>
    /// TODO
    /// </summary>
    void trackEvent();

    // Metodos eliminados por seguridad del patron singleton
    // Eliminado el operador de clon
    Tracker(Tracker& other) = delete;
    // Eliminado el operador de asignacion
    void operator=(const Tracker&) = delete;
};

