/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */
/*
  Piper C++

  An API for audio analysis and feature extraction plugins.

  Centre for Digital Music, Queen Mary, University of London.
  Copyright 2006-2016 Chris Cannam and QMUL.
  
  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the "Software"), to deal in the Software without
  restriction, including without limitation the rights to use, copy,
  modify, merge, publish, distribute, sublicense, and/or sell copies
  of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR
  ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  Except as contained in this notice, the names of the Centre for
  Digital Music; Queen Mary, University of London; and Chris Cannam
  shall not be used in advertising or otherwise to promote the sale,
  use or other dealings in this Software without prior written
  authorization.
*/

#ifndef PIPER_AUTO_PLUGIN_H
#define PIPER_AUTO_PLUGIN_H

#include "ProcessQtTransport.h"
#include "CapnpRRClient.h"

#include <cstdint>

namespace piper_vamp {
namespace client {

class AutoPlugin : public Vamp::Plugin
{
public:
    AutoPlugin(std::string serverName,
               std::string pluginKey,
               float inputSampleRate,
               int adapterFlags) :
        Vamp::Plugin(inputSampleRate),
        m_transport(serverName, "capnp"),
        m_client(&m_transport)
    {
        LoadRequest req;
        req.pluginKey = pluginKey;
        req.inputSampleRate = inputSampleRate;
        req.adapterFlags = adapterFlags;
        try {
            LoadResponse resp = m_client.loadPlugin(req);
            m_plugin = resp.plugin;
        } catch (ServerCrashed c) {
            std::cerr << c.what() << std::endl;
            m_plugin = 0;
        }
    }

    virtual ~AutoPlugin() {
        delete m_plugin;
    }

    bool isOK() const {
        return (m_plugin != nullptr);
    }
    
    virtual std::string getIdentifier() const {
        return getPlugin()->getIdentifier();
    }

    virtual std::string getName() const {
        return getPlugin()->getName();
    }

    virtual std::string getDescription() const {
        return getPlugin()->getDescription();
    }

    virtual std::string getMaker() const {
        return getPlugin()->getMaker();
    }

    virtual std::string getCopyright() const {
        return getPlugin()->getCopyright();
    }

    virtual int getPluginVersion() const {
        return getPlugin()->getPluginVersion();
    }

    virtual ParameterList getParameterDescriptors() const {
        return getPlugin()->getParameterDescriptors();
    }

    virtual float getParameter(std::string name) const {
        return getPlugin()->getParameter(name);
    }

    virtual void setParameter(std::string name, float value) {
        getPlugin()->setParameter(name, value);
    }

    virtual ProgramList getPrograms() const {
        return getPlugin()->getPrograms();
    }

    virtual std::string getCurrentProgram() const {
        return getPlugin()->getCurrentProgram();
    }
    
    virtual void selectProgram(std::string program) {
        getPlugin()->selectProgram(program);
    }

    virtual bool initialise(size_t inputChannels,
                            size_t stepSize,
                            size_t blockSize) {
        return getPlugin()->initialise(inputChannels, stepSize, blockSize);
    }

    virtual void reset() {
        getPlugin()->reset();
    }

    virtual InputDomain getInputDomain() const {
        return getPlugin()->getInputDomain();
    }

    virtual size_t getPreferredBlockSize() const {
        return getPlugin()->getPreferredBlockSize();
    }

    virtual size_t getPreferredStepSize() const {
        return getPlugin()->getPreferredStepSize();
    }

    virtual size_t getMinChannelCount() const {
        return getPlugin()->getMinChannelCount();
    }

    virtual size_t getMaxChannelCount() const {
        return getPlugin()->getMaxChannelCount();
    }

    virtual OutputList getOutputDescriptors() const {
        return getPlugin()->getOutputDescriptors();
    }

    virtual FeatureSet process(const float *const *inputBuffers,
                               Vamp::RealTime timestamp) {
        return getPlugin()->process(inputBuffers, timestamp);
    }

    virtual FeatureSet getRemainingFeatures() {
        return getPlugin()->getRemainingFeatures();
    }

private:
    ProcessQtTransport m_transport;
    CapnpRRClient m_client;
    Vamp::Plugin *m_plugin;
    Vamp::Plugin *getPlugin() const {
        if (!m_plugin) {
            throw std::logic_error
                ("Plugin load failed (should have called AutoPlugin::isOK)");
        }
        return m_plugin;
    }
};

}
}

#endif

    
