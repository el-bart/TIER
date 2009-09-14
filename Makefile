
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME=TEST.out
APP_NAME=vc_srv



all: rel

rel: CXXFLAGS=$(CXXFLAGS_REL)
rel: CXXLIBS=$(CXXLIBS_REL)
rel: TARGET=rel
rel: make_common

deb: CXXFLAGS=$(CXXFLAGS_DEB)
deb: CXXLIBS=$(CXXLIBS_DEB)
deb: TARGET=deb
deb: make_common

test: CXXFLAGS=$(CXXFLAGS_DEB)
test: CXXLIBS=$(CXXLIBS_DEB)
test: deb TEST.o
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME) Network/Server.o Network/TCPServer.o Network/TCPClient.o Network/Resolver.o VCCln/CGraphics/Segment.o VCCln/CGraphics/Raster.o VCCln/CGraphics/bitmap/bitmap.o VCCln/CGraphics/filter/histo_filter.o VCCln/CGraphics/filter/main_filters.o VCCln/CGraphics/filter/req_col_unif.o VCCln/CGraphics/filter/raster/scale_raster.o VCCln/CGraphics/filter/raster/histogram.o VCCln/CGraphics/filter/raster/stats.o VCCln/CGraphics/filter/raster/main_raster.o VCCln/CGraphics/filter/denoise.o VCCln/RasterDeserializer.o VCSrv/ServerApp.o VCSrv/Connection.o VCSrv/RasterSerializer.o VCSrv/Graphics/Histogram.o VCSrv/Graphics/V4LCapture.o VCSrv/Graphics/ProcessRaster.o Maps/DeepthTransformer.o Maps/VectorImg.o Maps/VectorImgFilter.o Maps/Vectorizer.o DebugWin/DebugTransmit.o DebugWin/DebugWin.o DebugWin/DebugRecieve.o Zuchter/Path/Planner.o Zuchter/Path/GlobalMap.o Zuchter/CommandCreator.o Zuchter/Zuchter.o Zuchter/MapCreator.o Zuchter/PathCreator.o Zuchter/CommProto/ProtoLocal.o Zuchter/CommProto/sterownik_uC/wabuf/wabuf.o Zuchter/CommProto/sterownik_uC/tier_low_io/portio/portio.o Zuchter/CommProto/sterownik_uC/tier_low_io/tier_low_io.o Zuchter/CommProto/sterownik_uC/tier_io/tier_io.o Zuchter/CommProto/sterownik_uC/tier_io/tier_pck/tier_pck.o Zuchter/TIERctl.o  TEST.o $(CXXLIBS)



make_common: 
	$(MAKE) -C Network/ $(TARGET)
	$(MAKE) -C VCCln/ $(TARGET)
	$(MAKE) -C VCSrv/ $(TARGET)
	$(MAKE) -C bitmap/ $(TARGET)
	$(MAKE) -C raster/ $(TARGET)
	$(MAKE) -C filter/ $(TARGET)
	$(MAKE) -C CGraphics/ $(TARGET)
	$(MAKE) -C Graphics/ $(TARGET)
	$(MAKE) -C Vectors/ $(TARGET)
	$(MAKE) -C Maps/ $(TARGET)
	$(MAKE) -C DebugWin/ $(TARGET)
	$(MAKE) -C Zuchter/ $(TARGET)
	$(MAKE) -C Path/ $(TARGET)
	$(MAKE) -C CommProto/ $(TARGET)
	ctags -R


clean:
	$(MAKE) -C Network/ clean
	$(MAKE) -C VCCln/ clean
	$(MAKE) -C VCSrv/ clean
	$(MAKE) -C bitmap/ clean
	$(MAKE) -C raster/ clean
	$(MAKE) -C filter/ clean
	$(MAKE) -C CGraphics/ clean
	$(MAKE) -C Graphics/ clean
	$(MAKE) -C Vectors/ clean
	$(MAKE) -C Maps/ clean
	$(MAKE) -C DebugWin/ clean
	$(MAKE) -C Zuchter/ clean
	$(MAKE) -C Path/ clean
	$(MAKE) -C CommProto/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f $(APP_NAME)
	rm -f gmon.out


