// KinectHeadTracking.cpp : Wrapper around the Kinect to get the Face/Eye Position
//

#include "stdafx.h"
#include "HeadTracker.h"

#include "Camera.h"

HeadTracker::HeadTracker(Camera & Camera)
	:BoundCamera(Camera)
{
}

void HeadTracker::Initialize()
{
	Utility::ThrowOnFail(GetDefaultKinectSensor(&KinectSensor));
	KinectSensor->Open();

	SetupBodyFrameReader();
	SetupHighDefinitionFaceFrameReader();
	SetupFaceModel();
}

void HeadTracker::Release()
{
	if (KinectSensor)
	{
		KinectSensor->Close();
		KinectSensor.Reset();
	}
}

void HeadTracker::Update()
{
	for (Event & Event : Events)
	{
		CheckEvent(Event);
	}
}

void HeadTracker::SetupBodyFrameReader()
{
	Utility::ThrowOnFail(KinectSensor->get_BodyFrameSource(&BodyFrameSource));

	INT32 BodyCount = 0;
	Utility::ThrowOnFail(BodyFrameSource->get_BodyCount(&BodyCount));
	Bodies.resize(BodyCount);

	Utility::ThrowOnFail(BodyFrameSource->OpenReader(&BodyFrameReader));

	AddEvent<IBodyFrameReader>(BodyFrameReader, &IBodyFrameReader::SubscribeFrameArrived, &HeadTracker::BodyFrameRecieved);
}

void HeadTracker::SetupHighDefinitionFaceFrameReader()
{
	Utility::ThrowOnFail(CreateHighDefinitionFaceFrameSource(KinectSensor.Get(), &HighDefinitionFaceFrameSource));
	Utility::ThrowOnFail(HighDefinitionFaceFrameSource->OpenReader(&HighDefinitionFaceFrameReader));

	AddEvent<IHighDefinitionFaceFrameReader>(HighDefinitionFaceFrameReader, &IHighDefinitionFaceFrameReader::SubscribeFrameArrived, &HeadTracker::HighDefinitionFaceFrameRecieved);
}

void HeadTracker::SetupFaceModel()
{
	float Deformation[FaceShapeDeformations_Count] = {};
	Utility::ThrowOnFail(CreateFaceModel(1.0f, FaceShapeDeformations_Count, Deformation, &FaceModel));
	Utility::ThrowOnFail(CreateFaceAlignment(&FaceAlignment));

	UINT32 VertexCount;
	Utility::ThrowOnFail(GetFaceModelVertexCount(&VertexCount));
	FaceVertices.resize(VertexCount);
}

void HeadTracker::CheckEvent(Event & Event)
{
	HANDLE EventHandle = reinterpret_cast<HANDLE>(Event.first);

	switch (WaitForSingleObject(EventHandle, 0))
	{
	case WAIT_TIMEOUT:
		// Non signaled, so there is no new data
		return;
	case WAIT_FAILED:
	{
		std::wstringstream Error(L"Error Code: ");
		Error << GetLastError();
		Utility::Throw(Error.str().c_str());
		return;
	}
	case WAIT_OBJECT_0:
		(this->*Event.second)(Event.first);
		return;
	}
}

void HeadTracker::BodyFrameRecieved(WAITABLE_HANDLE EventHandle)
{
	Microsoft::WRL::ComPtr<IBodyFrame> BodyFrame = GetBodyFrame(EventHandle);

	if (BodyFrame == nullptr)
	{
		return;
	}

	UpdateBodies(BodyFrame);
	UpdateTrackedBody();
}

void HeadTracker::HighDefinitionFaceFrameRecieved(WAITABLE_HANDLE EventHandle)
{
	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrame> FaceFrame = GetFaceFrame(EventHandle);

	if (FaceFrame == nullptr)
	{
		return;
	}

	BOOLEAN IsFaceTracket;
	Utility::ThrowOnFail(FaceFrame->get_IsFaceTracked(&IsFaceTracket));

	if (!IsFaceTracket)
	{
		return;
	}

	Utility::ThrowOnFail(FaceFrame->GetAndRefreshFaceAlignmentResult(FaceAlignment.Get()));
	Utility::ThrowOnFail(FaceModel->CalculateVerticesForAlignment(FaceAlignment.Get(), static_cast<UINT>(FaceVertices.size()), FaceVertices.data()));

	CameraSpacePoint & NoseTop = FaceVertices[HighDetailFacePoints_NoseTop];
	BoundCamera.UpdateCamera(Vector3(NoseTop.X, NoseTop.Y, NoseTop.Z));
}

Microsoft::WRL::ComPtr<IBodyFrame> HeadTracker::GetBodyFrame(WAITABLE_HANDLE EventHandle)
{
	Microsoft::WRL::ComPtr<IBodyFrameArrivedEventArgs> BodyFrameArrivedEventArgs;
	Microsoft::WRL::ComPtr<IBodyFrameReference> BodyFrameReference;
	Microsoft::WRL::ComPtr<IBodyFrame> BodyFrame;

	Utility::ThrowOnFail(BodyFrameReader->GetFrameArrivedEventData(EventHandle, &BodyFrameArrivedEventArgs));
	Utility::ThrowOnFail(BodyFrameArrivedEventArgs->get_FrameReference(&BodyFrameReference));
	BodyFrameReference->AcquireFrame(&BodyFrame);

	return BodyFrame;
}

void HeadTracker::UpdateBodies(Microsoft::WRL::ComPtr<IBodyFrame>& BodyFrame)
{
	std::vector<IBody *> NewBodyData(Bodies.size(), nullptr);
	std::transform(Bodies.begin(), Bodies.end(), NewBodyData.begin(), [](auto ComPtr)->auto { return ComPtr.Get(); });
	Utility::ThrowOnFail(BodyFrame->GetAndRefreshBodyData(static_cast<UINT>(NewBodyData.size()), NewBodyData.data()));
	std::transform(NewBodyData.begin(), NewBodyData.end(), Bodies.begin(), Bodies.begin(), [](auto RawPtr, auto ComPtr)->auto { return ComPtr = RawPtr; });
}

void HeadTracker::UpdateTrackedBody()
{
	if (!UpdateCurrentTrackedBody())
	{
		TrackNewBody();
	}
}

bool HeadTracker::UpdateCurrentTrackedBody()
{
	if (TrackedBody == nullptr)
	{
		return false;
	}

	UINT64 CurrentTrackingID;
	Utility::ThrowOnFail(TrackedBody->get_TrackingId(&CurrentTrackingID));

	auto IsTrackedBody = [=](auto Body)
		{
			BOOLEAN IsTracked;
			Utility::ThrowOnFail(Body->get_IsTracked(&IsTracked));
			if (!IsTracked)
			{
				return false;
			}

			UINT64 BodyTrackingID;
			Utility::ThrowOnFail(Body->get_TrackingId(&BodyTrackingID));

			return (CurrentTrackingID == BodyTrackingID);
		};

	auto Result = std::find_if(Bodies.begin(), Bodies.end(), IsTrackedBody);
	TrackedBody = (Result != Bodies.end()) ? (*Result) : nullptr;

	return (TrackedBody != nullptr);
}

void HeadTracker::TrackNewBody()
{
	UINT64 NewTrackingID = 0;

	for (auto & Body : Bodies)
	{
		BOOLEAN IsTracked;
		Utility::ThrowOnFail(Body->get_IsTracked(&IsTracked));
		if (IsTracked)
		{
			TrackedBody = Body;
			Utility::ThrowOnFail(Body->get_TrackingId(&NewTrackingID));
			break;
		}
	}

	Utility::ThrowOnFail(HighDefinitionFaceFrameSource->put_TrackingId(NewTrackingID));
}

Microsoft::WRL::ComPtr<IHighDefinitionFaceFrame> HeadTracker::GetFaceFrame(WAITABLE_HANDLE EventHandle)
{
	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrameArrivedEventArgs> HighDefinitionFaceFrameArrivedEventArgs;
	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrameReference> HighDefinitionFaceFrameReference;
	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrame> HighDefinitionFaceFrame;

	Utility::ThrowOnFail(HighDefinitionFaceFrameReader->GetFrameArrivedEventData(EventHandle, &HighDefinitionFaceFrameArrivedEventArgs));
	Utility::ThrowOnFail(HighDefinitionFaceFrameArrivedEventArgs->get_FrameReference(&HighDefinitionFaceFrameReference));
	HighDefinitionFaceFrameReference->AcquireFrame(&HighDefinitionFaceFrame);

	return HighDefinitionFaceFrame;
}

