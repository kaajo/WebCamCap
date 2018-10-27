TEMPLATE = subdirs

SUBDIRS =   WccSample \
            WebCamCapAPI \
            Calib \
            WebCamCap2 \
            WebCamCapInterfaces \
            WebCamCapPrimitives \
            WebCamCapGui

WccSample.depends += WebCamCapAPI

WebCamCapAPI.depends += WebCamCapPrimitives

WebCamCapGui.depends += WebCamCapPrimitives
WebCamCapInterfaces.depends += WebCamCapPrimitives

WebCamCap2.depends += WebCamCapGui WebCamCapInterfaces
