#include "SplineManager.hpp"
#include "ShaderPaths.hpp"
SplineManager::SplineManager(int totalFrames) :
	finishedAllSplines(false),
	currentSpline(0),
	mResolution(500),
	mTotalFrames(totalFrames),
	mCurrentFrame(0),
	mShowControlPoints(false),
	mShowCage(false),
	mShowSplinePoints(false),
	mShowSpline(true),
	mIsInterpolationDone(false)
{
	mTotalFrames = totalFrames;
	addSplines();

	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glGenBuffers(1, &mControlBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mControlBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * mAllSplinesControlPoints.size(), mAllSplinesControlPoints.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mSplineBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mSplineBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * mAllSplinePoints.size(), mAllSplinePoints.data(), GL_STATIC_DRAW);

	std::string shaderDir = generated::ShaderPaths::getShaderDirectory();

	std::vector<ShaderInfo> shaders
	{
		{ GL_VERTEX_SHADER, shaderDir + "spline.vs.glsl" },
		{ GL_FRAGMENT_SHADER, shaderDir + "spline.fs.glsl" }
	};

	mShaders.push_back(ShaderPointer(new Shader));
	mShaders[0]->compileShaders(shaders);
	mShaders[0]->linkShaders();

	GLuint var;
	var = mShaders[0]->getUniformVariable("uMVP");
	mUniforms.insert(UniformKey("uMVP", var));

	var = mShaders[0]->getUniformVariable("fColour");
	mUniforms.insert(UniformKey("fColour", var));

	mShaders[0]->disableShaders();
	glBindVertexArray(0);
}
SplineManager::~SplineManager() {
	glDeleteVertexArrays(1, &mVao);
	glDeleteVertexArrays(1, &mControlBuffer);
	glDeleteVertexArrays(1, &mSplineBuffer);
}
void SplineManager::addSplines() {
	mTempControlPoints = std::vector<Point>
	{
		{ -20, -5, 0 },
		{ -19, 5, -15 },
		{ 12.7f, -5, -1.4f },
		{ 20, 8.2f, 4.4f }
	};
	mSplines.push_back(Spline(mTotalFrames, mTempControlPoints));


	for (int i = 0; i < mSplines.size(); ++i) {
		mAllSplinesControlPoints.insert(mAllSplinesControlPoints.end(),mSplines[i].getControlPoints().begin(), mSplines[i].getControlPoints().end());
		mAllSplinePoints.insert(mAllSplinePoints.end(),mSplines[i].getSplinePoints().begin(), mSplines[i].getSplinePoints().end());
	}
}
void SplineManager::renderGeometry(atlas::math::Matrix4 projection, atlas::math::Matrix4 view) {
	USING_ATLAS_MATH_NS;

	mShaders[0]->enableShaders();

	glBindVertexArray(mVao);

	Matrix4 mvp = projection * view * mModel;
	glUniformMatrix4fv(mUniforms["uMVP"], 1, GL_FALSE, &mvp[0][0]);

	// Draw the control points first.
	glUniform3f(mUniforms["fColour"], 1, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mControlBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (mShowControlPoints)
	{
		glPointSize(5.0f);
		glDrawArrays(GL_POINTS, 0, GLsizei(mAllSplinesControlPoints.size()));
		glPointSize(1.0f);
	}

	if (mShowCage)
	{
		glDrawArrays(GL_LINE_STRIP, 0, GLsizei(mAllSplinesControlPoints.size()));
	}

	// Now draw the spline.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mSplineBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glUniform3f(mUniforms["fColour"], 0, 1, 0);

	if (mShowSpline)
	{
		glLineWidth(5.0f);
		glDrawArrays(GL_LINE_STRIP, 0, mResolution + 1);
		glLineWidth(1.0f);
	}

	if (mShowSplinePoints)
	{
		glPointSize(8.0f);
		glDrawArrays(GL_POINTS, 1, mResolution);
		glPointSize(1.0f);
	}

	glDisableVertexAttribArray(0);

	mShaders[0]->disableShaders();
}
void SplineManager::updateGeometry(atlas::utils::Time const& t) {
	mSplines[currentSpline].updateGeometry(t);
	if (mSplines[currentSpline].doneInterpolation()) {
		++currentSpline;
		if (currentSpline == mSplines.size()) {
			finishedAllSplines = true;
		}
	}
}

atlas::math::Point SplineManager::getSplinePosition() {
	return mSplines[currentSpline].getSplinePosition();
}
void SplineManager::showSpline() {
	//for (int i = 0; i < mSplines.size(); ++i) {
	//	mSplines[i].showSpline();
	//}
	mShowSpline = !mShowSpline;
}
void SplineManager::showControlPoints() {
	for (int i = 0; i < mSplines.size(); ++i) {
		mSplines[i].showControlPoints();
	}
	mShowControlPoints = !mShowControlPoints;
}
void SplineManager::showCage() {
	for (int i = 0; i < mSplines.size(); ++i) {
		mSplines[i].showCage();
	}
}
void SplineManager::showSplinePoints() {
	for (int i = 0; i < mSplines.size(); ++i) {
		mSplines[i].showSplinePoints();
	}
}
bool SplineManager::doneInterpolation() {
	return finishedAllSplines;
}