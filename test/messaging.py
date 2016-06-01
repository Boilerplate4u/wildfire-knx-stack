# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.8
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_messaging', [dirname(__file__)])
        except ImportError:
            import _messaging
            return _messaging
        if fp is not None:
            try:
                _mod = imp.load_module('_messaging', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _messaging = swig_import_helper()
    del swig_import_helper
else:
    import _messaging
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        if _newclass:
            object.__setattr__(self, name, value)
        else:
            self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0


class KnxMsg_StandardFrameType(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KnxMsg_StandardFrameType, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KnxMsg_StandardFrameType, name)
    __repr__ = _swig_repr
    __swig_setmethods__["ctrl"] = _messaging.KnxMsg_StandardFrameType_ctrl_set
    __swig_getmethods__["ctrl"] = _messaging.KnxMsg_StandardFrameType_ctrl_get
    if _newclass:
        ctrl = _swig_property(_messaging.KnxMsg_StandardFrameType_ctrl_get, _messaging.KnxMsg_StandardFrameType_ctrl_set)
    __swig_setmethods__["source"] = _messaging.KnxMsg_StandardFrameType_source_set
    __swig_getmethods__["source"] = _messaging.KnxMsg_StandardFrameType_source_get
    if _newclass:
        source = _swig_property(_messaging.KnxMsg_StandardFrameType_source_get, _messaging.KnxMsg_StandardFrameType_source_set)
    __swig_setmethods__["dest"] = _messaging.KnxMsg_StandardFrameType_dest_set
    __swig_getmethods__["dest"] = _messaging.KnxMsg_StandardFrameType_dest_get
    if _newclass:
        dest = _swig_property(_messaging.KnxMsg_StandardFrameType_dest_get, _messaging.KnxMsg_StandardFrameType_dest_set)
    __swig_setmethods__["npci"] = _messaging.KnxMsg_StandardFrameType_npci_set
    __swig_getmethods__["npci"] = _messaging.KnxMsg_StandardFrameType_npci_get
    if _newclass:
        npci = _swig_property(_messaging.KnxMsg_StandardFrameType_npci_get, _messaging.KnxMsg_StandardFrameType_npci_set)
    __swig_setmethods__["tpci"] = _messaging.KnxMsg_StandardFrameType_tpci_set
    __swig_getmethods__["tpci"] = _messaging.KnxMsg_StandardFrameType_tpci_get
    if _newclass:
        tpci = _swig_property(_messaging.KnxMsg_StandardFrameType_tpci_get, _messaging.KnxMsg_StandardFrameType_tpci_set)
    __swig_setmethods__["apci"] = _messaging.KnxMsg_StandardFrameType_apci_set
    __swig_getmethods__["apci"] = _messaging.KnxMsg_StandardFrameType_apci_get
    if _newclass:
        apci = _swig_property(_messaging.KnxMsg_StandardFrameType_apci_get, _messaging.KnxMsg_StandardFrameType_apci_set)
    __swig_setmethods__["data"] = _messaging.KnxMsg_StandardFrameType_data_set
    __swig_getmethods__["data"] = _messaging.KnxMsg_StandardFrameType_data_get
    if _newclass:
        data = _swig_property(_messaging.KnxMsg_StandardFrameType_data_get, _messaging.KnxMsg_StandardFrameType_data_set)

    def __init__(self):
        this = _messaging.new_KnxMsg_StandardFrameType()
        try:
            self.this.append(this)
        except Exception:
            self.this = this
    __swig_destroy__ = _messaging.delete_KnxMsg_StandardFrameType
    __del__ = lambda self: None
KnxMsg_StandardFrameType_swigregister = _messaging.KnxMsg_StandardFrameType_swigregister
KnxMsg_StandardFrameType_swigregister(KnxMsg_StandardFrameType)

class KnxMsg_PropertyFrameType(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KnxMsg_PropertyFrameType, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KnxMsg_PropertyFrameType, name)
    __repr__ = _swig_repr
    __swig_setmethods__["ctrl"] = _messaging.KnxMsg_PropertyFrameType_ctrl_set
    __swig_getmethods__["ctrl"] = _messaging.KnxMsg_PropertyFrameType_ctrl_get
    if _newclass:
        ctrl = _swig_property(_messaging.KnxMsg_PropertyFrameType_ctrl_get, _messaging.KnxMsg_PropertyFrameType_ctrl_set)
    __swig_setmethods__["source"] = _messaging.KnxMsg_PropertyFrameType_source_set
    __swig_getmethods__["source"] = _messaging.KnxMsg_PropertyFrameType_source_get
    if _newclass:
        source = _swig_property(_messaging.KnxMsg_PropertyFrameType_source_get, _messaging.KnxMsg_PropertyFrameType_source_set)
    __swig_setmethods__["dest"] = _messaging.KnxMsg_PropertyFrameType_dest_set
    __swig_getmethods__["dest"] = _messaging.KnxMsg_PropertyFrameType_dest_get
    if _newclass:
        dest = _swig_property(_messaging.KnxMsg_PropertyFrameType_dest_get, _messaging.KnxMsg_PropertyFrameType_dest_set)
    __swig_setmethods__["npci"] = _messaging.KnxMsg_PropertyFrameType_npci_set
    __swig_getmethods__["npci"] = _messaging.KnxMsg_PropertyFrameType_npci_get
    if _newclass:
        npci = _swig_property(_messaging.KnxMsg_PropertyFrameType_npci_get, _messaging.KnxMsg_PropertyFrameType_npci_set)
    __swig_setmethods__["tpci"] = _messaging.KnxMsg_PropertyFrameType_tpci_set
    __swig_getmethods__["tpci"] = _messaging.KnxMsg_PropertyFrameType_tpci_get
    if _newclass:
        tpci = _swig_property(_messaging.KnxMsg_PropertyFrameType_tpci_get, _messaging.KnxMsg_PropertyFrameType_tpci_set)
    __swig_setmethods__["apci"] = _messaging.KnxMsg_PropertyFrameType_apci_set
    __swig_getmethods__["apci"] = _messaging.KnxMsg_PropertyFrameType_apci_get
    if _newclass:
        apci = _swig_property(_messaging.KnxMsg_PropertyFrameType_apci_get, _messaging.KnxMsg_PropertyFrameType_apci_set)
    __swig_setmethods__["obj_id"] = _messaging.KnxMsg_PropertyFrameType_obj_id_set
    __swig_getmethods__["obj_id"] = _messaging.KnxMsg_PropertyFrameType_obj_id_get
    if _newclass:
        obj_id = _swig_property(_messaging.KnxMsg_PropertyFrameType_obj_id_get, _messaging.KnxMsg_PropertyFrameType_obj_id_set)
    __swig_setmethods__["prop_id"] = _messaging.KnxMsg_PropertyFrameType_prop_id_set
    __swig_getmethods__["prop_id"] = _messaging.KnxMsg_PropertyFrameType_prop_id_get
    if _newclass:
        prop_id = _swig_property(_messaging.KnxMsg_PropertyFrameType_prop_id_get, _messaging.KnxMsg_PropertyFrameType_prop_id_set)
    __swig_setmethods__["num_elems"] = _messaging.KnxMsg_PropertyFrameType_num_elems_set
    __swig_getmethods__["num_elems"] = _messaging.KnxMsg_PropertyFrameType_num_elems_get
    if _newclass:
        num_elems = _swig_property(_messaging.KnxMsg_PropertyFrameType_num_elems_get, _messaging.KnxMsg_PropertyFrameType_num_elems_set)
    __swig_setmethods__["start_index"] = _messaging.KnxMsg_PropertyFrameType_start_index_set
    __swig_getmethods__["start_index"] = _messaging.KnxMsg_PropertyFrameType_start_index_get
    if _newclass:
        start_index = _swig_property(_messaging.KnxMsg_PropertyFrameType_start_index_get, _messaging.KnxMsg_PropertyFrameType_start_index_set)
    __swig_setmethods__["data"] = _messaging.KnxMsg_PropertyFrameType_data_set
    __swig_getmethods__["data"] = _messaging.KnxMsg_PropertyFrameType_data_get
    if _newclass:
        data = _swig_property(_messaging.KnxMsg_PropertyFrameType_data_get, _messaging.KnxMsg_PropertyFrameType_data_set)

    def __init__(self):
        this = _messaging.new_KnxMsg_PropertyFrameType()
        try:
            self.this.append(this)
        except Exception:
            self.this = this
    __swig_destroy__ = _messaging.delete_KnxMsg_PropertyFrameType
    __del__ = lambda self: None
KnxMsg_PropertyFrameType_swigregister = _messaging.KnxMsg_PropertyFrameType_swigregister
KnxMsg_PropertyFrameType_swigregister(KnxMsg_PropertyFrameType)

class KnxMsg_PollingFrameType(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KnxMsg_PollingFrameType, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KnxMsg_PollingFrameType, name)
    __repr__ = _swig_repr
    __swig_setmethods__["ctrl"] = _messaging.KnxMsg_PollingFrameType_ctrl_set
    __swig_getmethods__["ctrl"] = _messaging.KnxMsg_PollingFrameType_ctrl_get
    if _newclass:
        ctrl = _swig_property(_messaging.KnxMsg_PollingFrameType_ctrl_get, _messaging.KnxMsg_PollingFrameType_ctrl_set)
    __swig_setmethods__["source"] = _messaging.KnxMsg_PollingFrameType_source_set
    __swig_getmethods__["source"] = _messaging.KnxMsg_PollingFrameType_source_get
    if _newclass:
        source = _swig_property(_messaging.KnxMsg_PollingFrameType_source_get, _messaging.KnxMsg_PollingFrameType_source_set)
    __swig_setmethods__["poll_addr"] = _messaging.KnxMsg_PollingFrameType_poll_addr_set
    __swig_getmethods__["poll_addr"] = _messaging.KnxMsg_PollingFrameType_poll_addr_get
    if _newclass:
        poll_addr = _swig_property(_messaging.KnxMsg_PollingFrameType_poll_addr_get, _messaging.KnxMsg_PollingFrameType_poll_addr_set)
    __swig_setmethods__["num_slots"] = _messaging.KnxMsg_PollingFrameType_num_slots_set
    __swig_getmethods__["num_slots"] = _messaging.KnxMsg_PollingFrameType_num_slots_get
    if _newclass:
        num_slots = _swig_property(_messaging.KnxMsg_PollingFrameType_num_slots_get, _messaging.KnxMsg_PollingFrameType_num_slots_set)
    __swig_setmethods__["slot"] = _messaging.KnxMsg_PollingFrameType_slot_set
    __swig_getmethods__["slot"] = _messaging.KnxMsg_PollingFrameType_slot_get
    if _newclass:
        slot = _swig_property(_messaging.KnxMsg_PollingFrameType_slot_get, _messaging.KnxMsg_PollingFrameType_slot_set)

    def __init__(self):
        this = _messaging.new_KnxMsg_PollingFrameType()
        try:
            self.this.append(this)
        except Exception:
            self.this = this
    __swig_destroy__ = _messaging.delete_KnxMsg_PollingFrameType
    __del__ = lambda self: None
KnxMsg_PollingFrameType_swigregister = _messaging.KnxMsg_PollingFrameType_swigregister
KnxMsg_PollingFrameType_swigregister(KnxMsg_PollingFrameType)

class KnxMsg_Buffer(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KnxMsg_Buffer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KnxMsg_Buffer, name)
    __repr__ = _swig_repr
    __swig_setmethods__["next"] = _messaging.KnxMsg_Buffer_next_set
    __swig_getmethods__["next"] = _messaging.KnxMsg_Buffer_next_get
    if _newclass:
        next = _swig_property(_messaging.KnxMsg_Buffer_next_get, _messaging.KnxMsg_Buffer_next_set)
    __swig_setmethods__["len"] = _messaging.KnxMsg_Buffer_len_set
    __swig_getmethods__["len"] = _messaging.KnxMsg_Buffer_len_get
    if _newclass:
        len = _swig_property(_messaging.KnxMsg_Buffer_len_get, _messaging.KnxMsg_Buffer_len_set)
    __swig_setmethods__["service"] = _messaging.KnxMsg_Buffer_service_set
    __swig_getmethods__["service"] = _messaging.KnxMsg_Buffer_service_get
    if _newclass:
        service = _swig_property(_messaging.KnxMsg_Buffer_service_get, _messaging.KnxMsg_Buffer_service_set)
    __swig_setmethods__["sap"] = _messaging.KnxMsg_Buffer_sap_set
    __swig_getmethods__["sap"] = _messaging.KnxMsg_Buffer_sap_get
    if _newclass:
        sap = _swig_property(_messaging.KnxMsg_Buffer_sap_get, _messaging.KnxMsg_Buffer_sap_set)
    __swig_setmethods__["status"] = _messaging.KnxMsg_Buffer_status_set
    __swig_getmethods__["status"] = _messaging.KnxMsg_Buffer_status_get
    if _newclass:
        status = _swig_property(_messaging.KnxMsg_Buffer_status_get, _messaging.KnxMsg_Buffer_status_set)
    __swig_getmethods__["msg"] = _messaging.KnxMsg_Buffer_msg_get
    if _newclass:
        msg = _swig_property(_messaging.KnxMsg_Buffer_msg_get)

    def __init__(self):
        this = _messaging.new_KnxMsg_Buffer()
        try:
            self.this.append(this)
        except Exception:
            self.this = this
    __swig_destroy__ = _messaging.delete_KnxMsg_Buffer
    __del__ = lambda self: None
KnxMsg_Buffer_swigregister = _messaging.KnxMsg_Buffer_swigregister
KnxMsg_Buffer_swigregister(KnxMsg_Buffer)

class tagKnxMsg_Buffer_msg(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, tagKnxMsg_Buffer_msg, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, tagKnxMsg_Buffer_msg, name)
    __repr__ = _swig_repr
    __swig_setmethods__["raw"] = _messaging.tagKnxMsg_Buffer_msg_raw_set
    __swig_getmethods__["raw"] = _messaging.tagKnxMsg_Buffer_msg_raw_get
    if _newclass:
        raw = _swig_property(_messaging.tagKnxMsg_Buffer_msg_raw_get, _messaging.tagKnxMsg_Buffer_msg_raw_set)
    __swig_setmethods__["standard"] = _messaging.tagKnxMsg_Buffer_msg_standard_set
    __swig_getmethods__["standard"] = _messaging.tagKnxMsg_Buffer_msg_standard_get
    if _newclass:
        standard = _swig_property(_messaging.tagKnxMsg_Buffer_msg_standard_get, _messaging.tagKnxMsg_Buffer_msg_standard_set)
    __swig_setmethods__["property"] = _messaging.tagKnxMsg_Buffer_msg_property_set
    __swig_getmethods__["property"] = _messaging.tagKnxMsg_Buffer_msg_property_get
    if _newclass:
        property = _swig_property(_messaging.tagKnxMsg_Buffer_msg_property_get, _messaging.tagKnxMsg_Buffer_msg_property_set)
    __swig_setmethods__["polling"] = _messaging.tagKnxMsg_Buffer_msg_polling_set
    __swig_getmethods__["polling"] = _messaging.tagKnxMsg_Buffer_msg_polling_get
    if _newclass:
        polling = _swig_property(_messaging.tagKnxMsg_Buffer_msg_polling_get, _messaging.tagKnxMsg_Buffer_msg_polling_set)

    def __init__(self):
        this = _messaging.new_tagKnxMsg_Buffer_msg()
        try:
            self.this.append(this)
        except Exception:
            self.this = this
    __swig_destroy__ = _messaging.delete_tagKnxMsg_Buffer_msg
    __del__ = lambda self: None
tagKnxMsg_Buffer_msg_swigregister = _messaging.tagKnxMsg_Buffer_msg_swigregister
tagKnxMsg_Buffer_msg_swigregister(tagKnxMsg_Buffer_msg)


def KnxMsg_Init():
    return _messaging.KnxMsg_Init()
KnxMsg_Init = _messaging.KnxMsg_Init

def KnxMsg_AllocateBuffer(buffer):
    return _messaging.KnxMsg_AllocateBuffer(buffer)
KnxMsg_AllocateBuffer = _messaging.KnxMsg_AllocateBuffer

def KnxMsg_AllocateBufferWrapper():
    return _messaging.KnxMsg_AllocateBufferWrapper()
KnxMsg_AllocateBufferWrapper = _messaging.KnxMsg_AllocateBufferWrapper

def KnxMsg_ReleaseBuffer(ptr):
    return _messaging.KnxMsg_ReleaseBuffer(ptr)
KnxMsg_ReleaseBuffer = _messaging.KnxMsg_ReleaseBuffer

def KnxMsg_ClearBuffer(ptr):
    return _messaging.KnxMsg_ClearBuffer(ptr)
KnxMsg_ClearBuffer = _messaging.KnxMsg_ClearBuffer

def KnxMsg_Post(ptr):
    return _messaging.KnxMsg_Post(ptr)
KnxMsg_Post = _messaging.KnxMsg_Post

def KnxMsg_Get(task):
    return _messaging.KnxMsg_Get(task)
KnxMsg_Get = _messaging.KnxMsg_Get

def KnxMsg_RedirectToUser(layer):
    return _messaging.KnxMsg_RedirectToUser(layer)
KnxMsg_RedirectToUser = _messaging.KnxMsg_RedirectToUser

def KnxMsg_SetLen(pBuffer, len):
    return _messaging.KnxMsg_SetLen(pBuffer, len)
KnxMsg_SetLen = _messaging.KnxMsg_SetLen

def KnxMsg_GetLen(pBuffer):
    return _messaging.KnxMsg_GetLen(pBuffer)
KnxMsg_GetLen = _messaging.KnxMsg_GetLen

def KnxMsg_GetRoutingCount(pBuffer):
    return _messaging.KnxMsg_GetRoutingCount(pBuffer)
KnxMsg_GetRoutingCount = _messaging.KnxMsg_GetRoutingCount

def KnxMsg_SetRoutingCount(pBuffer):
    return _messaging.KnxMsg_SetRoutingCount(pBuffer)
KnxMsg_SetRoutingCount = _messaging.KnxMsg_SetRoutingCount

def KnxMsg_SetRoutingCtrl(pBuffer, on):
    return _messaging.KnxMsg_SetRoutingCtrl(pBuffer, on)
KnxMsg_SetRoutingCtrl = _messaging.KnxMsg_SetRoutingCtrl

def KnxMsg_GetRoutingCtrl(pBuffer):
    return _messaging.KnxMsg_GetRoutingCtrl(pBuffer)
KnxMsg_GetRoutingCtrl = _messaging.KnxMsg_GetRoutingCtrl
# This file is compatible with both classic and new-style classes.

